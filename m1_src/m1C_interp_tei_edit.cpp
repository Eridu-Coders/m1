/**
 * @file m1C_interp_tei_edit.cpp
 * @brief Rich text editor for TEI texts (TEI_Container::get_edit_widget() and its machinery)
 *
 * The TEI sub-tree under a TEI_Container is mapped onto a QTextDocument displayed in a QTextEdit:
 *
 * - <div>                         --> QTextFrame (the root div itself uses the document's root frame)
 * - <p>, <quote>, <head>, <l>, ...  --> one or more QTextBlock (several when a block-level element such
 *                                     as a <list> or a <cit> interrupts the paragraph -- "continuation" blocks)
 * - <list>/<item>                 --> QTextList
 * - <table>/<row>/<cell>          --> QTextTable
 * - <figure>                      --> image (QTextImageFormat) + caption block
 * - leaf chunks                   --> runs of characters (one or more QTextFragment)
 * - <note>                        --> a single atomic inline object drawn as "[n]" (TEI_NoteMarkerHandler);
 *                                     clicking it opens a pop-up editor for the note's own content
 *
 * Back-links to the graph:
 * - blocks: TEI_BlockData (QTextBlockUserData) holding the ItemID of the owning block-level node
 * - characters: custom QTextCharFormat properties (TEIEdit::PropKind / PropItemId / PropField). As two
 *   different chunks never have identical formats, Qt never merges them into a single fragment
 * - frames: PropItemId property of the QTextFrameFormat
 *
 * ItemIDs are stored (not Interp pointers), so that the Interp cache can be invalidated freely.
 *
 * Phase 1 editing model: only the text of existing leaf chunks and the PCTLF/PCTRT separators between
 * chunks can be edited; the structure is locked (no Enter, no deletion across block or chunk boundaries,
 * no deletion of a whole chunk, plain-text paste only, no drag and drop). Saving walks the document,
 * regroups the text by chunk ID and only writes what changed. A structural integrity check (block count,
 * synthetic characters) guards against anything that slipped through the key filtering.
 */
#include "m1C_interp.h"
#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_lv2_iterators.h"
#include "m1B_store.h"

#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>
#include <QTextFrame>
#include <QTextTable>
#include <QTextList>
#include <QTextObjectInterface>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QDialog>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QMenu>
#include <QContextMenuEvent>
#include <QKeyEvent>
#include <QInputMethodEvent>
#include <QMimeData>
#include <QScrollBar>
#include <QImage>
#include <QUrl>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QTabWidget>
#include <QFontDatabase>
#include <QMap>
#include <QSet>
#include <QRegularExpression>
#include <algorithm>

namespace M1MidPlane{

namespace TEIEdit{
    /// custom QTextFormat properties
    enum Property{
        PropKind = QTextFormat::UserProperty + 1,   ///< Kind (int)
        PropItemId,                                 ///< ItemID of the chunk (text, separator), note (marker) or div (frame)
        PropField,                                  ///< SeparatorField (separators only)
        PropNoteN                                   ///< note number (note markers only)
    };
    /// what a run of characters is
    enum Kind{
        KindNone = 0,       ///< not part of the TEI content (e.g. empty padding blocks around frames)
        KindText = 1,       ///< text of a leaf chunk -- editable
        KindSeparator = 2,  ///< PCTLF / PCTRT field of a chunk -- editable
        KindSynthetic = 3   ///< added for display only (quotation marks, note markers, images) -- locked
    };
    enum SeparatorField{ SepLeading = 0, SepTrailing = 1 };

    const int NoteObjectType = QTextFormat::UserObject + 1;

    /// tags of the container chunks whose label is "[tag] joined text" (recomputed when their text changes)
    const QSet<QString> g_relabel_tags{"name", "term", "title", "q", "bibl", "cit"};
}

/** --------------------------------------------------------------- TEI_BlockData ---------------------------------
 * @brief back-link from a QTextBlock to the block-level graph node it belongs to
 */
class TEI_BlockData : public QTextBlockUserData{
public:
    TEI_BlockData(M1Env::ItemID p_owner, bool p_continuation) : m_owner(p_owner), m_continuation(p_continuation){}
    M1Env::ItemID m_owner;      ///< <p>, <quote>, <item>, <cell>, <head>, <l>, ...
    bool m_continuation;        ///< true if not the first block of its owner (e.g. paragraph text resumed after a <list>)
};

/** --------------------------------------------------------------- TEI_DocBuilder ---------------------------------
 * @brief builds the QTextDocument from a TEI sub-tree. Driven by the TEI_*::buildTextDoc() overrides
 *
 * Blocks are opened lazily: a block-level node only pushes an "owner" and the block is created when
 * the first piece of inline content arrives (or at the end, if the node produced no block at all).
 */
class TEI_DocBuilder{
public:
    struct TextRecord{
        QString m_original;
        QList<M1Env::ItemID> m_ancestors;   ///< enclosing chunks whose label must be recomputed if this text changes
    };
    typedef QPair<M1Env::ItemID, int> SepKey;

    TEI_DocBuilder(QTextDocument* p_doc) : m_doc(p_doc), m_cursor(p_doc){}

    void build(TEI_Container* p_root){
        m_root = p_root;
        m_style_stack.append(QTextCharFormat());
        beginBlockOwner(p_root->myself(), QTextBlockFormat());
        buildNode(p_root);
        endBlockOwner();
        m_block_count = m_doc->blockCount();
    }
    bool isRoot(const Interp* p_interp) const {return p_interp == m_root;}

    // ------------------------------- tree traversal
    /// recursion entry point for every node (guards against cycles: see the heading remark in buildContent())
    void buildNode(TEI_Container* p_node){
        M1Env::ItemID l_id = p_node->myself()->item_id();
        if(m_path.contains(l_id)) return;
        m_path.insert(l_id);
        p_node->buildTextDoc(*this);
        m_path.remove(l_id);
    }

    /// heading + descendants, joined with their PCTLF/PCTRT separators (same logic as TEI_Container::baseText())
    void buildChildren(TEI_Container* p_container){
        std::shared_ptr<Interp> l_heading = p_container->headingInterp();
        if(l_heading != nullptr && !m_path.contains(l_heading->myself()->item_id()))
            if(TEI_Container* l_hc = dynamic_cast<TEI_Container*>(l_heading.get())) buildNode(l_hc);

        bool l_first = true;
        for(const auto& l_child : p_container->descendants()){
            // highlights and other non-TEI descendants are not part of the editable text
            TEI_Container* l_tei = dynamic_cast<TEI_Container*>(l_child.get());
            if(l_tei == nullptr) continue;
            TEI_ChunkInterp* l_chunk = dynamic_cast<TEI_ChunkInterp*>(l_tei);
            bool l_inline = l_chunk != nullptr && !l_chunk->isBlockLevel();
            M1Store::Item_lv2* l_item = l_tei->myself();

            // a leading space before a block makes no sense --> not displayed (and therefore left untouched)
            if(!l_first && l_inline) insertSeparator(l_item, TEIEdit::SepLeading, l_item->getField(M1Env::PCTLF_SIID));
            buildNode(l_tei);
            insertSeparator(l_item, TEIEdit::SepTrailing, l_item->getField(M1Env::PCTRT_SIID));
            l_first = false;
        }
    }

    /// children if any, otherwise the node's own label (leaf chunk). Same fallback as TEI_Container::baseText()
    void buildContent(TEI_Container* p_node){
        // NB: TEI_Container::initialize() assigns m_heading from *any* edge leading to a <head> chunk,
        // including the BLNGS edge from a chunk inside a <head> back to it --> a heading already on the
        // current path is not a real heading
        std::shared_ptr<Interp> l_heading = p_node->headingInterp();
        bool l_has_heading = l_heading != nullptr && !m_path.contains(l_heading->myself()->item_id());
        if(p_node->descendants().empty() && !l_has_heading){
            if(dynamic_cast<TEI_ChunkInterp*>(p_node) != nullptr)
                insertChunkText(p_node->myself(), p_node->myself()->text());
        }
        else buildChildren(p_node);
    }

    // ------------------------------- block-level structure
    void beginBlockOwner(M1Store::Item_lv2* p_owner, const QTextBlockFormat& p_fmt, const QTextCharFormat& p_style = QTextCharFormat()){
        QTextBlockFormat l_fmt = m_owner_stack.isEmpty() ? QTextBlockFormat() : m_owner_stack.last().m_fmt;
        l_fmt.merge(p_fmt);
        m_owner_stack.append(OwnerLevel{p_owner->item_id(), l_fmt, false, m_blocks_created});
        pushStyle(p_style);
        m_inline_open = false;
    }
    void endBlockOwner(){
        // a node that produced no block at all still gets an (empty) one, so that it is visible
        if(m_blocks_created == m_owner_stack.last().m_blocks_at_start) openBlock();
        popStyle();
        m_owner_stack.removeLast();
        m_inline_open = false;
    }

    void beginFrame(M1Store::Item_lv2* p_owner){
        QTextFrameFormat l_ff;
        l_ff.setBorder(1);
        l_ff.setBorderBrush(QColor(200, 200, 200));
        l_ff.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
        l_ff.setPadding(6);
        l_ff.setTopMargin(4);
        l_ff.setBottomMargin(4);
        l_ff.setProperty(TEIEdit::PropItemId, QVariant::fromValue(p_owner->item_id()));
        m_frame_stack.append(m_cursor.currentFrame());
        m_cursor.insertFrame(l_ff);
        m_fresh_block = true;
        m_inline_open = false;
    }
    void endFrame(){
        // the document is always built by appending --> the end of the parent frame is just after this one
        m_cursor = m_frame_stack.takeLast()->lastCursorPosition();
        m_fresh_block = true;
        m_inline_open = false;
    }

    void buildTable(M1Store::Item_lv2* p_table, const QList<TEI_Container*>& p_rows){
        int l_cols = 0;
        QList<QList<TEI_Container*>> l_cells;
        for(TEI_Container* l_row : p_rows){
            QList<TEI_Container*> l_row_cells;
            for(const auto& l_d : l_row->descendants())
                if(TEI_CellInterp* l_cell = dynamic_cast<TEI_CellInterp*>(l_d.get())) l_row_cells.append(l_cell);
            l_cols = std::max(l_cols, (int)l_row_cells.size());
            l_cells.append(l_row_cells);
        }
        if(l_cells.isEmpty() || l_cols == 0) return;

        QTextTableFormat l_tf;
        l_tf.setBorder(1);
        l_tf.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
        l_tf.setBorderCollapse(true);
        l_tf.setCellPadding(4);
        l_tf.setCellSpacing(0);
        if(p_table != nullptr) l_tf.setProperty(TEIEdit::PropItemId, QVariant::fromValue(p_table->item_id()));

        QTextFrame* l_parent = m_cursor.currentFrame();
        QTextTable* l_table = m_cursor.insertTable(l_cells.size(), l_cols, l_tf);
        for(int r = 0; r < l_cells.size(); r++)
            for(int c = 0; c < l_cells[r].size(); c++){
                m_cursor = l_table->cellAt(r, c).firstCursorPosition();
                m_fresh_block = true;
                m_inline_open = false;
                buildNode(l_cells[r][c]);
            }
        m_cursor = l_parent->lastCursorPosition();
        m_fresh_block = true;
        m_inline_open = false;
    }

    void beginList(M1Store::Item_lv2* p_list, QTextListFormat::Style p_style){
        QTextListFormat l_lf;
        l_lf.setStyle(p_style);
        l_lf.setIndent(m_list_stack.size() + 1);
        l_lf.setProperty(TEIEdit::PropItemId, QVariant::fromValue(p_list->item_id()));
        m_list_stack.append(ListLevel{nullptr, l_lf});
    }
    void endList(){ m_list_stack.removeLast(); m_pending_list_item = false; }
    /// the next block opened becomes a bullet of the current list
    void markListItem(){ m_pending_list_item = true; }

    // ------------------------------- inline content
    void pushStyle(const QTextCharFormat& p_delta){
        QTextCharFormat l_fmt = m_style_stack.isEmpty() ? QTextCharFormat() : m_style_stack.last();
        l_fmt.merge(p_delta);
        m_style_stack.append(l_fmt);
    }
    void popStyle(){ m_style_stack.removeLast(); }
    void pushAncestor(M1Env::ItemID p_id){ m_ancestor_stack.append(p_id); }
    void popAncestor(){ m_ancestor_stack.removeLast(); }

    void insertChunkText(M1Store::Item_lv2* p_chunk, const QString& p_text){
        ensureInline();
        QTextCharFormat l_fmt = m_style_stack.last();
        l_fmt.setProperty(TEIEdit::PropKind, TEIEdit::KindText);
        l_fmt.setProperty(TEIEdit::PropItemId, QVariant::fromValue(p_chunk->item_id()));
        m_cursor.insertText(p_text, l_fmt);
        m_text_records.insert(p_chunk->item_id(), TextRecord{p_text, m_ancestor_stack});
    }
    void insertSeparator(M1Store::Item_lv2* p_chunk, TEIEdit::SeparatorField p_field, const QString& p_value){
        if(p_value.isEmpty()) return;
        ensureInline();
        QTextCharFormat l_fmt = m_style_stack.last();
        l_fmt.setProperty(TEIEdit::PropKind, TEIEdit::KindSeparator);
        l_fmt.setProperty(TEIEdit::PropItemId, QVariant::fromValue(p_chunk->item_id()));
        l_fmt.setProperty(TEIEdit::PropField, p_field);
        m_cursor.insertText(p_value, l_fmt);
        m_sep_records.insert(SepKey(p_chunk->item_id(), p_field), p_value);
    }
    void insertSynthetic(const QString& p_text){
        ensureInline();
        QTextCharFormat l_fmt = m_style_stack.last();
        l_fmt.setProperty(TEIEdit::PropKind, TEIEdit::KindSynthetic);
        m_cursor.insertText(p_text, l_fmt);
        m_synthetic_len += p_text.length();
    }
    void insertNoteMarker(M1Store::Item_lv2* p_note, const QString& p_n){
        ensureInline();
        QTextCharFormat l_fmt = m_style_stack.last();
        l_fmt.setObjectType(TEIEdit::NoteObjectType);
        l_fmt.setProperty(TEIEdit::PropKind, TEIEdit::KindSynthetic);
        l_fmt.setProperty(TEIEdit::PropItemId, QVariant::fromValue(p_note->item_id()));
        l_fmt.setProperty(TEIEdit::PropNoteN, p_n);
        m_cursor.insertText(QString(QChar::ObjectReplacementCharacter), l_fmt);
        m_synthetic_len += 1;
    }
    void insertImage(const QString& p_path){
        QImage l_img(p_path);
        if(l_img.isNull()){
            insertSynthetic(QString("[image not found: %1]").arg(p_path));
            return;
        }
        if(l_img.width() > 600) l_img = l_img.scaledToWidth(600, Qt::SmoothTransformation);
        m_doc->addResource(QTextDocument::ImageResource, QUrl(p_path), l_img);
        ensureInline();
        QTextImageFormat l_fmt;
        l_fmt.setName(p_path);
        l_fmt.setWidth(l_img.width());
        l_fmt.setHeight(l_img.height());
        l_fmt.setProperty(TEIEdit::PropKind, TEIEdit::KindSynthetic);
        m_cursor.insertImage(l_fmt);
        m_synthetic_len += 1;
    }

    // ------------------------------- results
    QMap<M1Env::ItemID, TextRecord> m_text_records;
    QMap<SepKey, QString> m_sep_records;
    int m_synthetic_len = 0;
    int m_block_count = 0;

private:
    struct OwnerLevel{
        M1Env::ItemID m_id;
        QTextBlockFormat m_fmt;
        bool m_has_block;
        int m_blocks_at_start;
    };
    struct ListLevel{
        QTextList* m_list;
        QTextListFormat m_fmt;
    };

    void ensureInline(){ if(!m_inline_open) openBlock(); }
    void openBlock(){
        OwnerLevel& l_top = m_owner_stack.last();
        QTextBlockFormat l_fmt = l_top.m_fmt;
        // continuation blocks inside a list item are aligned with the item text
        if(!m_list_stack.isEmpty() && !m_pending_list_item) l_fmt.setIndent(m_list_stack.size());

        if(m_fresh_block) m_cursor.setBlockFormat(l_fmt);
        else m_cursor.insertBlock(l_fmt, QTextCharFormat());
        m_cursor.block().setUserData(new TEI_BlockData(l_top.m_id, l_top.m_has_block));
        l_top.m_has_block = true;

        if(m_pending_list_item && !m_list_stack.isEmpty()){
            ListLevel& l_list = m_list_stack.last();
            if(l_list.m_list == nullptr) l_list.m_list = m_cursor.createList(l_list.m_fmt);
            else l_list.m_list->add(m_cursor.block());
            m_pending_list_item = false;
        }
        m_blocks_created++;
        m_fresh_block = false;
        m_inline_open = true;
    }

    QTextDocument* m_doc;
    QTextCursor m_cursor;
    TEI_Container* m_root = nullptr;

    bool m_fresh_block = true;      ///< the block under the cursor is empty and unclaimed (can be adopted)
    bool m_inline_open = false;     ///< the block under the cursor accepts inline content of the current owner
    bool m_pending_list_item = false;
    int m_blocks_created = 0;

    QList<OwnerLevel> m_owner_stack;
    QList<QTextCharFormat> m_style_stack;
    QList<ListLevel> m_list_stack;
    QList<QTextFrame*> m_frame_stack;
    QList<M1Env::ItemID> m_ancestor_stack;
    QSet<M1Env::ItemID> m_path;
};

/** --------------------------------------------------------------- TEI_NoteMarkerHandler ---------------------------------
 * @brief draws the note marker inline object as a small blue "[n]"
 */
class TEI_NoteMarkerHandler : public QObject, public QTextObjectInterface{
    Q_OBJECT
    Q_INTERFACES(QTextObjectInterface)
public:
    TEI_NoteMarkerHandler(QObject* p_parent) : QObject(p_parent){}

    QSizeF intrinsicSize(QTextDocument* p_doc, int, const QTextFormat& p_format) override{
        QFontMetricsF l_fm(font(p_doc, p_format));
        return QSizeF(l_fm.horizontalAdvance(label(p_format)) + 2, l_fm.height());
    }
    void drawObject(QPainter* p_painter, const QRectF& p_rect, QTextDocument* p_doc, int, const QTextFormat& p_format) override{
        p_painter->save();
        p_painter->setFont(font(p_doc, p_format));
        p_painter->setPen(QColor(0, 0, 200));
        p_painter->drawText(p_rect, Qt::AlignLeft | Qt::AlignTop, label(p_format));
        p_painter->restore();
    }
private:
    static QString label(const QTextFormat& p_format){
        QString l_n = p_format.property(TEIEdit::PropNoteN).toString();
        return QString("[%1]").arg(l_n.isEmpty() ? "note" : l_n);
    }
    /// format font completed by the document's default font (which carries the editor font size)
    static QFont font(QTextDocument* p_doc, const QTextFormat& p_format){
        QFont l_font = p_format.toCharFormat().font().resolve(p_doc->defaultFont());
        l_font.setItalic(false);
        if(l_font.pointSizeF() > 0) l_font.setPointSizeF(l_font.pointSizeF() * 0.8);
        return l_font;
    }
};

/** --------------------------------------------------------------- TEI_TextEdit ---------------------------------
 * @brief QTextEdit restricted to phase 1 editing (text of existing chunks and separators only)
 */
class TEI_TextEdit : public QTextEdit{
    Q_OBJECT
public:
    TEI_TextEdit(M1Store::Item_lv2* p_root, QWidget* p_parent) : QTextEdit(p_parent), m_root_item(p_root){
        setAcceptDrops(false);
        setTabChangesFocus(true);
        setOverwriteMode(false);
        viewport()->setMouseTracking(true);

        // black on white whatever the UI style/theme: palette for plain styles, style sheet for
        // styles that ignore the palette (or when an application-wide style sheet is in use)
        QPalette l_pal = palette();
        for(QPalette::ColorGroup l_group : {QPalette::Active, QPalette::Inactive, QPalette::Disabled}){
            l_pal.setColor(l_group, QPalette::Base, Qt::white);
            l_pal.setColor(l_group, QPalette::Text, Qt::black);
            l_pal.setColor(l_group, QPalette::Window, Qt::white);
            l_pal.setColor(l_group, QPalette::WindowText, Qt::black);
        }
        setPalette(l_pal);
        viewport()->setPalette(l_pal);
        viewport()->setAutoFillBackground(true);
        setStyleSheet("QTextEdit { background-color: white; color: black; }");

        m_note_handler = new TEI_NoteMarkerHandler(this);
        m_ui_font_size = document()->defaultFont().pointSizeF();
        applyFontSize(M1Store::StorageStatic::teiEditorFontSize());
        rebuild();
    }

public slots:
    /// display font size in points (0 = UI default). Editor setting only: nothing is stored in the text
    void applyFontSize(int p_size){
        QFont l_font = document()->defaultFont();
        if(p_size > 0) l_font.setPointSize(p_size);
        else if(m_ui_font_size > 0) l_font.setPointSizeF(m_ui_font_size);
        document()->setDefaultFont(l_font);
    }

    /// (re)builds the document from the graph, discarding unsaved changes
    void rebuild(){
        std::shared_ptr<Interp> l_interp = Interp::getInterp(m_root_item);
        TEI_Container* l_root = dynamic_cast<TEI_Container*>(l_interp.get());
        if(l_root == nullptr) return;
        m_root = l_interp;

        QTextDocument* l_doc = document();
        l_doc->setUndoRedoEnabled(false);
        l_doc->clear();
        l_doc->documentLayout()->registerHandler(TEIEdit::NoteObjectType, m_note_handler);
        TEI_DocBuilder l_builder(l_doc);
        l_builder.build(l_root);
        m_text_records = l_builder.m_text_records;
        m_sep_records = l_builder.m_sep_records;
        m_synthetic_len = l_builder.m_synthetic_len;
        m_block_count = l_builder.m_block_count;
        l_doc->setUndoRedoEnabled(true);
        l_doc->setModified(false);
        moveCursor(QTextCursor::Start);
    }

    /// writes changed chunk texts / separators to the graph. Returns false if nothing could be saved
    bool saveToGraph(){
        QMap<M1Env::ItemID, QString> l_texts;
        QMap<TEI_DocBuilder::SepKey, QString> l_seps;
        int l_synthetic_len = 0;
        for(QTextBlock l_block = document()->begin(); l_block.isValid(); l_block = l_block.next())
            for(QTextBlock::iterator l_it = l_block.begin(); !l_it.atEnd(); ++l_it){
                QTextFragment l_frag = l_it.fragment();
                if(!l_frag.isValid()) continue;
                QTextCharFormat l_fmt = l_frag.charFormat();
                M1Env::ItemID l_id = l_fmt.property(TEIEdit::PropItemId).toULongLong();
                switch(l_fmt.intProperty(TEIEdit::PropKind)){
                case TEIEdit::KindText: l_texts[l_id] += l_frag.text(); break;
                case TEIEdit::KindSeparator: l_seps[TEI_DocBuilder::SepKey(l_id, l_fmt.intProperty(TEIEdit::PropField))] += l_frag.text(); break;
                case TEIEdit::KindSynthetic: l_synthetic_len += l_frag.length(); break;
                default: break;
                }
            }

        if(document()->blockCount() != m_block_count || l_synthetic_len != m_synthetic_len){
            QMessageBox::warning(this, "TEI editor",
                                 "The structure of the text has been altered (this should not happen in this editor).\n"
                                 "Nothing was saved. Use Revert to reload the text from the database.");
            return false;
        }

        bool l_changed = false;
        QSet<M1Env::ItemID> l_relabel;
        for(auto l_it = m_text_records.begin(); l_it != m_text_records.end(); ++l_it){
            QString l_new = l_texts.value(l_it.key());
            if(l_new == l_it.value().m_original) continue;
            M1Store::Item_lv2* l_chunk = M1Store::Item_lv2::getExisting(l_it.key());
            qCDebug(g_cat_tmp_spotlight).noquote() << "TEI edit, chunk text:" << l_chunk->dbgShort() << "-->" << l_new;
            l_chunk->setText_lv1(l_new);
            l_it.value().m_original = l_new;
            for(M1Env::ItemID l_anc : l_it.value().m_ancestors) l_relabel.insert(l_anc);
            l_changed = true;
        }
        for(auto l_it = m_sep_records.begin(); l_it != m_sep_records.end(); ++l_it){
            QString l_new = l_seps.value(l_it.key());
            if(l_new == l_it.value()) continue;
            M1Store::Item_lv2* l_chunk = M1Store::Item_lv2::getExisting(l_it.key().first);
            M1Env::SpecialItemID l_field = l_it.key().second == TEIEdit::SepLeading ? M1Env::PCTLF_SIID : M1Env::PCTRT_SIID;
            qCDebug(g_cat_tmp_spotlight).noquote() << "TEI edit, separator:" << l_chunk->dbgShort() << l_it.key().second << "-->" << l_new;
            setOrClearField(l_chunk, l_field, l_new);
            l_it.value() = l_new;
            l_changed = true;
        }
        // "[tag] joined text" labels of the enclosing <name>, <term>, <title>, <q>, <bibl>, <cit>
        for(M1Env::ItemID l_id : l_relabel){
            M1Store::Item_lv2* l_item = M1Store::Item_lv2::getExisting(l_id);
            TEI_ChunkInterp* l_chunk = dynamic_cast<TEI_ChunkInterp*>(Interp::getInterp(l_item).get());
            if(l_chunk == nullptr) continue;
            QString l_label = QString("[%1] %2").arg(l_chunk->chunkTag()).arg(joinedText(l_chunk));
            if(l_label != l_item->text()) l_item->setText_lv1(l_label);
        }

        document()->setModified(false);
        if(l_changed || m_refresh_pending){
            m_refresh_pending = false;
            emit graphUpdated();
        }
        return true;
    }

signals:
    /// the graph was modified (tree display and HTML view must be refreshed)
    void graphUpdated();

private slots:
    /// a note was saved from the pop-up editor. Refreshing now would replace this editor (and lose
    /// unsaved edits) --> deferred until this editor's own save if it has pending changes
    void noteSaved(){
        if(document()->isModified()) m_refresh_pending = true;
        else emit graphUpdated();
    }

protected:
    void keyPressEvent(QKeyEvent* p_event) override{
        if(p_event->key() == Qt::Key_Return || p_event->key() == Qt::Key_Enter || p_event->key() == Qt::Key_Insert) return;

        if(p_event->matches(QKeySequence::Cut)){
            QTextCursor l_cur = textCursor();
            if(l_cur.hasSelection() && analyzeRange(l_cur.selectionStart(), l_cur.selectionEnd(), false).m_ok){
                copy();
                l_cur.removeSelectedText();
            }
            return;
        }

        QTextCursor::MoveOperation l_move = QTextCursor::NoMove;
        bool l_whole_line = false;
        if(p_event->matches(QKeySequence::Delete)) l_move = QTextCursor::NextCharacter;
        else if(p_event->matches(QKeySequence::DeleteEndOfWord)) l_move = QTextCursor::NextWord;
        else if(p_event->matches(QKeySequence::DeleteStartOfWord)) l_move = QTextCursor::PreviousWord;
        else if(p_event->matches(QKeySequence::DeleteEndOfLine)) l_move = QTextCursor::EndOfBlock;
        else if(p_event->matches(QKeySequence::DeleteCompleteLine)) l_whole_line = true;
        else if(p_event->matches(QKeySequence::Backspace) || p_event->key() == Qt::Key_Backspace) l_move = QTextCursor::PreviousCharacter;
        if(l_move != QTextCursor::NoMove || l_whole_line){
            QTextCursor l_cur = textCursor();
            if(!l_cur.hasSelection()){
                if(l_whole_line){
                    l_cur.movePosition(QTextCursor::StartOfBlock);
                    l_cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
                }
                else l_cur.movePosition(l_move, QTextCursor::KeepAnchor);
            }
            if(l_cur.hasSelection() && analyzeRange(l_cur.selectionStart(), l_cur.selectionEnd(), false).m_ok)
                l_cur.removeSelectedText();
            return;
        }

        QString l_text = p_event->text();
        if(!l_text.isEmpty() && std::all_of(l_text.begin(), l_text.end(), [](QChar c){ return c.isPrint(); })){
            insertChecked(l_text);
            return;
        }
        // navigation, copy, paste (--> insertFromMimeData()), undo/redo, select all, ...
        QTextEdit::keyPressEvent(p_event);
    }

    void inputMethodEvent(QInputMethodEvent* p_event) override{
        if(p_event->commitString().isEmpty()){
            QTextEdit::inputMethodEvent(p_event);
            return;
        }
        insertChecked(p_event->commitString());
        QInputMethodEvent l_preedit_only(p_event->preeditString(), p_event->attributes());
        QTextEdit::inputMethodEvent(&l_preedit_only);
    }

    bool canInsertFromMimeData(const QMimeData* p_source) const override{ return p_source->hasText(); }
    void insertFromMimeData(const QMimeData* p_source) override{
        if(p_source->hasText()) insertChecked(p_source->text());
    }

    void contextMenuEvent(QContextMenuEvent* p_event) override{
        QMenu l_menu(this);
        l_menu.addAction("Undo", this, &QTextEdit::undo)->setEnabled(document()->isUndoAvailable());
        l_menu.addAction("Redo", this, &QTextEdit::redo)->setEnabled(document()->isRedoAvailable());
        l_menu.addSeparator();
        l_menu.addAction("Copy", this, &QTextEdit::copy)->setEnabled(textCursor().hasSelection());
        l_menu.addAction("Paste", this, &QTextEdit::paste);
        l_menu.addSeparator();
        l_menu.addAction("Select All", this, &QTextEdit::selectAll);
        l_menu.addSeparator();
        M1Env::ItemID l_id = nodeAt(p_event->pos());
        // one entry per node from the one under the mouse up to the root of this editor
        QMenu* l_debug_menu = l_menu.addMenu("Debug Print");
        M1Store::Item_lv2* l_node = l_id == M1Store::G_VOID_ITEM_ID ? nullptr : M1Store::Item_lv2::getExisting(l_id);
        for(int l_guard = 0; l_node != nullptr && l_guard < 64; l_guard++){
            M1Env::ItemID l_node_id = l_node->item_id();
            l_debug_menu->addAction(debugMenuLabel(l_node), this, [this, l_node_id](){ debugPrint(l_node_id); });
            if(l_node == m_root_item) break;
            M1Store::Item_lv2* l_owns = owningEdge(l_node);
            l_node = l_owns == nullptr ? nullptr : l_owns->getOrigin_lv2();
        }
        l_debug_menu->setEnabled(!l_debug_menu->isEmpty());
        l_menu.exec(p_event->globalPos());
    }

    void mousePressEvent(QMouseEvent* p_event) override{
        if(p_event->button() == Qt::LeftButton){
            M1Env::ItemID l_note = noteAt(p_event->position());
            if(l_note != M1Store::G_VOID_ITEM_ID){
                openNoteEditor(l_note);
                return;
            }
        }
        QTextEdit::mousePressEvent(p_event);
    }
    void mouseMoveEvent(QMouseEvent* p_event) override{
        viewport()->setCursor(noteAt(p_event->position()) != M1Store::G_VOID_ITEM_ID ? Qt::PointingHandCursor : Qt::IBeamCursor);
        QTextEdit::mouseMoveEvent(p_event);
    }

private:
    struct RangeInfo{
        bool m_ok = false;
        bool m_single_chunk = false;        ///< the range lies within the text of a single chunk
        QTextCharFormat m_chunk_fmt;        ///< format of that chunk (if m_single_chunk)
    };

    /**
     * @brief can the characters [p_start, p_end[ be removed?
     *
     * Not if the range crosses a block boundary, touches synthetic text or removes the whole text of a
     * chunk -- except, when p_replacing, if the range is entirely within one chunk (typing over a selection)
     */
    RangeInfo analyzeRange(int p_start, int p_end, bool p_replacing) const{
        RangeInfo l_ret;
        if(p_start >= p_end) return l_ret;
        QTextBlock l_block = document()->findBlock(p_start);
        if(p_end > l_block.position() + l_block.length() - 1) return l_ret;

        QMap<M1Env::ItemID, QPair<int, int>> l_counts;     // chunk --> (total length, removed length)
        QSet<M1Env::ItemID> l_touched;
        bool l_only_text = true;
        for(QTextBlock::iterator l_it = l_block.begin(); !l_it.atEnd(); ++l_it){
            QTextFragment l_frag = l_it.fragment();
            if(!l_frag.isValid()) continue;
            int l_fs = l_frag.position();
            int l_fe = l_fs + l_frag.length();
            int l_overlap = std::max(0, std::min(l_fe, p_end) - std::max(l_fs, p_start));
            QTextCharFormat l_fmt = l_frag.charFormat();
            int l_kind = l_fmt.intProperty(TEIEdit::PropKind);
            if(l_overlap > 0 && (l_kind == TEIEdit::KindSynthetic || l_kind == TEIEdit::KindNone)) return l_ret;
            if(l_kind == TEIEdit::KindText){
                M1Env::ItemID l_id = l_fmt.property(TEIEdit::PropItemId).toULongLong();
                l_counts[l_id].first += l_frag.length();
                l_counts[l_id].second += l_overlap;
                if(l_overlap > 0){
                    l_touched.insert(l_id);
                    l_ret.m_chunk_fmt = l_fmt;
                }
            }
            else if(l_overlap > 0) l_only_text = false;
        }
        l_ret.m_single_chunk = l_only_text && l_touched.size() == 1;
        for(const auto& l_count : l_counts)
            if(l_count.second == l_count.first && !(p_replacing && l_ret.m_single_chunk)) return l_ret;
        l_ret.m_ok = true;
        return l_ret;
    }

    /**
     * @brief graph node under p_pos (viewport coordinates): the chunk (text or separator) or note marker
     * under the mouse, otherwise the block-level node owning the block. G_VOID_ITEM_ID if none
     */
    M1Env::ItemID nodeAt(const QPoint& p_pos) const{
        int l_p = cursorForPosition(p_pos).position();
        // the cursor position is a boundary between two characters --> try the one after, then the one before
        for(int l_q : {l_p, l_p - 1}){
            if(l_q < 0) continue;
            QTextCharFormat l_fmt = charAt(l_q);
            if(l_fmt.hasProperty(TEIEdit::PropItemId)) return l_fmt.property(TEIEdit::PropItemId).toULongLong();
        }
        if(TEI_BlockData* l_data = dynamic_cast<TEI_BlockData*>(document()->findBlock(l_p).userData()))
            return l_data->m_owner;
        return M1Store::G_VOID_ITEM_ID;
    }

    /// OWNS edge from the parent of p_item (reciprocal of its BLNGS edge), nullptr if none
    static M1Store::Item_lv2* owningEdge(M1Store::Item_lv2* p_item){
        for(M1Store::Item_lv2_iterator l_it = p_item->getIteratorAuto(); !l_it.beyondEnd(); l_it.next())
            if(l_it.at()->isFullEdge() && l_it.at()->isOfType(M1Env::BLNGS_SIID) && l_it.at()->hasReciprocal()){
                M1Store::Item_lv2* l_reciprocal = l_it.at()->getReciprocalEdge_lv2();
                if(l_reciprocal->isOfType(M1Env::OWNS_SIID)) return l_reciprocal;
            }
        return nullptr;
    }

    /// "<tag> text" for chunks, label for the other nodes
    static QString debugMenuLabel(M1Store::Item_lv2* p_item){
        std::shared_ptr<Interp> l_interp = Interp::getInterp(p_item);
        QString l_text = M1Store::StorageStatic::maxLengthChop(QString(p_item->text()).replace(g_re_space, " "), 40);
        l_text.replace("&", "&&");  // no menu mnemonics
        if(TEI_ChunkInterp* l_chunk = dynamic_cast<TEI_ChunkInterp*>(l_interp.get()))
            return QString("<%1> %2").arg(l_chunk->chunkTag()).arg(l_text);
        return l_text;
    }

    /// non-modal window showing the HTML representation (baseText()) of a node, as source and rendered
    void debugPrint(M1Env::ItemID p_id){
        M1Store::Item_lv2* l_item = M1Store::Item_lv2::getExisting(p_id);
        std::shared_ptr<Interp> l_interp = Interp::getInterp(l_item);
        QString l_html = l_interp->baseText();
        QString l_debug_html = l_interp->debugHtml(owningEdge(l_item));

        QDialog* l_dialog = new QDialog(this);
        l_dialog->setAttribute(Qt::WA_DeleteOnClose);
        l_dialog->setWindowTitle(QString("Debug Print: %1").arg(l_item->dbgShort()));
        l_dialog->resize(800, 500);
        QVBoxLayout* l_layout = new QVBoxLayout(l_dialog);

        QLabel* l_header = new QLabel(l_item->dbgShort(), l_dialog);
        l_header->setTextInteractionFlags(Qt::TextSelectableByMouse);
        l_header->setWordWrap(true);
        l_layout->addWidget(l_header);

        QTabWidget* l_tabs = new QTabWidget(l_dialog);
        QPlainTextEdit* l_source = new QPlainTextEdit(l_html, l_tabs);
        l_source->setReadOnly(true);
        l_source->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
        l_tabs->addTab(l_source, "HTML source");
        QTextBrowser* l_rendered = new QTextBrowser(l_tabs);
        l_rendered->setHtml(l_html);
        l_tabs->addTab(l_rendered, "Rendered");
        QTextBrowser* l_debug = new QTextBrowser(l_tabs);
        l_debug->setHtml(l_debug_html);
        l_tabs->addTab(l_debug, "Debug info");
        l_layout->addWidget(l_tabs);

        QDialogButtonBox* l_buttons = new QDialogButtonBox(QDialogButtonBox::Close, l_dialog);
        connect(l_buttons, &QDialogButtonBox::rejected, l_dialog, &QDialog::close);
        l_layout->addWidget(l_buttons);
        l_dialog->show();
    }

    /// format of the character at p_pos (empty format if none)
    QTextCharFormat charAt(int p_pos) const{
        QTextBlock l_block = document()->findBlock(p_pos);
        for(QTextBlock::iterator l_it = l_block.begin(); !l_it.atEnd(); ++l_it){
            QTextFragment l_frag = l_it.fragment();
            if(l_frag.isValid() && p_pos >= l_frag.position() && p_pos < l_frag.position() + l_frag.length())
                return l_frag.charFormat();
        }
        return QTextCharFormat();
    }

    /**
     * @brief format to give to text typed at p_pos: that of the chunk text before it, otherwise of the chunk
     * text after it, otherwise of an adjacent separator. False if none (e.g. between two synthetic objects)
     */
    bool formatForInsertion(int p_pos, QTextCharFormat& p_fmt) const{
        QTextBlock l_block = document()->findBlock(p_pos);
        QTextCharFormat l_before = p_pos > l_block.position() ? charAt(p_pos - 1) : QTextCharFormat();
        QTextCharFormat l_after = p_pos < l_block.position() + l_block.length() - 1 ? charAt(p_pos) : QTextCharFormat();
        int l_kb = l_before.intProperty(TEIEdit::PropKind);
        int l_ka = l_after.intProperty(TEIEdit::PropKind);
        if(l_kb == TEIEdit::KindText) p_fmt = l_before;
        else if(l_ka == TEIEdit::KindText) p_fmt = l_after;
        else if(l_kb == TEIEdit::KindSeparator) p_fmt = l_before;
        else if(l_ka == TEIEdit::KindSeparator) p_fmt = l_after;
        else return false;
        return true;
    }

    /// inserts text typed or pasted, replacing the selection if any, within the phase 1 constraints
    void insertChecked(QString p_text){
        static const QRegularExpression l_re_breaks(R"([\r\n\x{2028}\x{2029}\t]+)");
        p_text.replace(l_re_breaks, " ");
        if(p_text.isEmpty()) return;

        QTextCursor l_cur = textCursor();
        QTextCharFormat l_fmt;
        if(l_cur.hasSelection()){
            RangeInfo l_info = analyzeRange(l_cur.selectionStart(), l_cur.selectionEnd(), true);
            if(!l_info.m_ok) return;
            if(l_info.m_single_chunk) l_fmt = l_info.m_chunk_fmt;
            else if(!formatForInsertion(l_cur.selectionStart(), l_fmt)) return;
        }
        else if(!formatForInsertion(l_cur.position(), l_fmt)) return;

        l_cur.beginEditBlock();
        l_cur.removeSelectedText();
        l_cur.insertText(p_text, l_fmt);
        l_cur.endEditBlock();
        setTextCursor(l_cur);
    }

    /// ItemID of the note whose marker is under p_pos (viewport coordinates), G_VOID_ITEM_ID otherwise
    M1Env::ItemID noteAt(const QPointF& p_pos) const{
        QPointF l_doc_pos = p_pos + QPointF(horizontalScrollBar()->value(), verticalScrollBar()->value());
        int l_p = document()->documentLayout()->hitTest(l_doc_pos, Qt::ExactHit);
        if(l_p < 0) return M1Store::G_VOID_ITEM_ID;
        // the hit position is the nearest character boundary --> the marker is either just after or just before
        for(int l_q : {l_p, l_p - 1}){
            if(l_q < 0) continue;
            QTextCharFormat l_fmt = charAt(l_q);
            if(l_fmt.objectType() == TEIEdit::NoteObjectType) return l_fmt.property(TEIEdit::PropItemId).toULongLong();
        }
        return M1Store::G_VOID_ITEM_ID;
    }

    void openNoteEditor(M1Env::ItemID p_note);

    /// joined text of a container chunk, same rules as TEIStackLevel::appendText() in the importer
    static QString joinedText(TEI_Container* p_container){
        static const QString l_glued_punct = QStringLiteral(".,;:!?)]}»›’”");
        QString l_ret;
        auto l_append = [&](const QString& p_text){
            if(p_text.isEmpty()) return;
            if(!l_ret.isEmpty() && !l_glued_punct.contains(p_text.at(0))) l_ret += " ";
            l_ret += p_text;
        };
        for(const auto& l_child : p_container->descendants()){
            TEI_ChunkInterp* l_chunk = dynamic_cast<TEI_ChunkInterp*>(l_child.get());
            if(l_chunk == nullptr) continue;
            const QString& l_tag = l_chunk->chunkTag();
            if(l_tag == "note" || l_tag == "l" || l_tag == "lg" || l_tag == "head") {}
            else if(l_chunk->descendants().empty()) l_append(QString(l_chunk->myself()->text()).trimmed());
            else l_append(joinedText(l_chunk));
            l_append(l_chunk->myself()->getField(M1Env::PCTRT_SIID));
        }
        return l_ret;
    }

    /// field edges are only unlinked, never deleted (they are recycled by the GC)
    static void setOrClearField(M1Store::Item_lv2* p_item, const M1Env::SpecialItemID p_field_type, const QString& p_value){
        M1Store::Item_lv2* l_field_edge = p_item->getFieldEdge(p_field_type);
        if(p_value.length() == 0){
            if(l_field_edge != nullptr) l_field_edge->unlinkEdge();
        }
        else if(l_field_edge == nullptr || l_field_edge->text() != p_value)
            p_item->setFieldEdge(p_value, p_field_type);
    }

    M1Store::Item_lv2* m_root_item;
    qreal m_ui_font_size = 0;           ///< default font size from the UI style (used when the setting is 0)
    std::shared_ptr<Interp> m_root;     ///< keeps the root Interp alive while the editor exists
    TEI_NoteMarkerHandler* m_note_handler;

    QMap<M1Env::ItemID, TEI_DocBuilder::TextRecord> m_text_records;
    QMap<TEI_DocBuilder::SepKey, QString> m_sep_records;
    int m_synthetic_len = 0;
    int m_block_count = 0;
    bool m_refresh_pending = false;
};

/** --------------------------------------------------------------- TEI_NoteDialog ---------------------------------
 * @brief pop-up editor for the content of a <note> (itself a TEI_TextEdit rooted at the note chunk,
 * so notes within notes work the same way)
 */
class TEI_NoteDialog : public QDialog{
    Q_OBJECT
public:
    TEI_NoteDialog(M1Store::Item_lv2* p_note, QWidget* p_parent) : QDialog(p_parent){
        QString l_n = p_note->getField(M1Env::TEI_N_ATTR_SIID);
        setWindowTitle(QString("Note %1").arg(l_n));
        resize(600, 300);

        QVBoxLayout* l_layout = new QVBoxLayout(this);
        m_edit = new TEI_TextEdit(p_note, this);
        l_layout->addWidget(m_edit);

        QDialogButtonBox* l_buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
        l_layout->addWidget(l_buttons);
        connect(l_buttons, &QDialogButtonBox::accepted, this, [this](){ if(m_edit->saveToGraph()) accept(); });
        connect(l_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }
    TEI_TextEdit* editor(){return m_edit;}

    void reject() override{
        if(m_edit->document()->isModified() &&
           QMessageBox::question(this, "Note", "Discard the changes made to this note?") != QMessageBox::Yes)
            return;
        QDialog::reject();
    }
private:
    TEI_TextEdit* m_edit;
};

void TEI_TextEdit::openNoteEditor(M1Env::ItemID p_note){
    M1Store::Item_lv2* l_note = M1Store::Item_lv2::getExisting(p_note);
    TEI_NoteDialog l_dialog(l_note, this);
    connect(l_dialog.editor(), &TEI_TextEdit::graphUpdated, this, &TEI_TextEdit::noteSaved);
    l_dialog.exec();
}

} // namespace M1MidPlane

/** --------------------------------------------------------------- TEI_Container ---------------------------------
 * @brief M1MidPlane::TEI_Container::get_edit_widget: Save / Revert bar + TEI_TextEdit
 */
QWidget* M1MidPlane::TEI_Container::get_edit_widget(){
    qCDebug(g_cat_interp_base).noquote() << "TEI_Container::get_edit_widget()" << m_myself->dbgShort();

    QWidget* l_panel = new QWidget();
    QVBoxLayout* l_layout = new QVBoxLayout(l_panel);

    QWidget* l_bar = new QWidget(l_panel);
    QHBoxLayout* l_bar_layout = new QHBoxLayout(l_bar);
    l_bar_layout->setContentsMargins(0, 0, 0, 0);
    QPushButton* l_save = new QPushButton("Save", l_bar);
    QPushButton* l_revert = new QPushButton("Revert", l_bar);
    QLabel* l_status = new QLabel(l_bar);
    // display font size (editor setting stored in the LMDB util table, not part of the TEI text)
    QSpinBox* l_font_size = new QSpinBox(l_bar);
    l_font_size->setRange(0, 48);
    l_font_size->setSpecialValueText("Default");
    l_font_size->setSuffix(" pt");
    l_font_size->setToolTip("Editor font size (display only)");
    l_font_size->setValue(M1Store::StorageStatic::teiEditorFontSize());
    l_bar_layout->addWidget(l_save);
    l_bar_layout->addWidget(l_revert);
    l_bar_layout->addWidget(l_status);
    l_bar_layout->addStretch(1);
    l_bar_layout->addWidget(new QLabel("Font size:", l_bar));
    l_bar_layout->addWidget(l_font_size);
    l_layout->addWidget(l_bar);

    TEI_TextEdit* l_edit = new TEI_TextEdit(m_myself, l_panel);
    l_layout->addWidget(l_edit);

    QObject::connect(l_save, &QPushButton::clicked, l_edit, &TEI_TextEdit::saveToGraph);
    QObject::connect(l_font_size, &QSpinBox::valueChanged, l_edit, [l_edit](int p_size){
        l_edit->applyFontSize(p_size);
        M1Store::StorageStatic::setTeiEditorFontSize(p_size);
    });
    QObject::connect(l_revert, &QPushButton::clicked, l_edit, [l_edit](){
        if(!l_edit->document()->isModified() ||
           QMessageBox::question(l_edit, "Revert", "Discard all unsaved changes?") == QMessageBox::Yes)
            l_edit->rebuild();
    });
    QObject::connect(l_edit->document(), &QTextDocument::modificationChanged, l_status, [l_status](bool p_modified){
        l_status->setText(p_modified ? "<i>modified</i>" : "");
    });
    QObject::connect(l_edit, &TEI_TextEdit::graphUpdated, this, [this](){
        if(m_tree_row != nullptr) m_tree_row->performPostUpdate();
    });

    return l_panel;
}

/**
 * @brief default: a block-level node holding its heading and descendants (<p>, <cell>, root <note>, ...)
 */
void M1MidPlane::TEI_Container::buildTextDoc(TEI_DocBuilder& p_builder){
    QTextBlockFormat l_bf;
    l_bf.setBottomMargin(6);
    p_builder.beginBlockOwner(m_myself, l_bf);
    p_builder.buildContent(this);
    p_builder.endBlockOwner();
}

/** --------------------------------------------------------------- TEI_DivInterp ---------------------------------
 * @brief nested divs are frames, the root div uses the document's root frame
 */
void M1MidPlane::TEI_DivInterp::buildTextDoc(TEI_DocBuilder& p_builder){
    bool l_root = p_builder.isRoot(this);
    if(!l_root) p_builder.beginFrame(m_myself);
    p_builder.beginBlockOwner(m_myself, QTextBlockFormat());
    p_builder.buildChildren(this);
    p_builder.endBlockOwner();
    if(!l_root) p_builder.endFrame();
}

/** --------------------------------------------------------------- TEI_ChunkInterp ---------------------------------
 * @brief block-level chunks (head, lg, l, cit) get their own block(s), the others are inline runs of text
 */
bool M1MidPlane::TEI_ChunkInterp::isBlockLevel() const{
    return m_chunk_tag == "head" || m_chunk_tag == "lg" || m_chunk_tag == "l" || m_chunk_tag == "cit";
}

void M1MidPlane::TEI_ChunkInterp::buildTextDoc(TEI_DocBuilder& p_builder){
    // <note>: atomic "[n]" marker in the text, its content is edited in a pop-up (unless it is the root)
    if(m_chunk_tag == "note"){
        if(p_builder.isRoot(this)) TEI_Container::buildTextDoc(p_builder);
        else p_builder.insertNoteMarker(m_myself, m_myself->getField(M1Env::TEI_N_ATTR_SIID));
        return;
    }

    bool l_relabel = TEIEdit::g_relabel_tags.contains(m_chunk_tag) && !descendants().empty();
    if(l_relabel) p_builder.pushAncestor(m_myself->item_id());

    if(isBlockLevel()){
        QTextBlockFormat l_bf;
        QTextCharFormat l_cf;
        if(m_chunk_tag == "head"){
            l_cf.setFontWeight(QFont::Bold);
            l_bf.setTopMargin(6);
            l_bf.setBottomMargin(6);
        }
        else if(m_chunk_tag == "lg"){
            l_bf.setLeftMargin(30);
            l_cf.setFontItalic(true);
        }
        else if(m_chunk_tag == "l") l_bf.setBottomMargin(0);
        else if(m_chunk_tag == "cit") l_bf.setBottomMargin(6);
        p_builder.beginBlockOwner(m_myself, l_bf, l_cf);
        p_builder.buildContent(this);
        p_builder.endBlockOwner();
    }
    else{
        QTextCharFormat l_cf;
        QString l_rend = m_myself->getField(M1Env::TEI_REND_ATTR_SIID);
        if(m_chunk_tag == "name") l_cf.setBackground(QColor("#fdd"));
        else if(m_chunk_tag == "term") l_cf.setBackground(QColor("#ddf"));
        else if(m_chunk_tag == "title") l_cf.setFontItalic(true);
        else if(m_chunk_tag == "q" && l_rend == "italic") l_cf.setFontItalic(true);
        else if(m_chunk_tag == "hi"){
            if(l_rend == "bold") l_cf.setFontWeight(QFont::Bold);
            else if(l_rend == "underline") l_cf.setFontUnderline(true);
            else if(l_rend == "sup") l_cf.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
            else if(l_rend == "sub") l_cf.setVerticalAlignment(QTextCharFormat::AlignSubScript);
            else l_cf.setFontItalic(true);
        }
        else if(m_chunk_tag == "ref"){
            QString l_target = m_myself->getField(M1Env::TEI_TARGET_ATTR_SIID);
            if(l_target.length() > 0){
                l_cf.setAnchor(true);
                l_cf.setAnchorHref(l_target);
                l_cf.setForeground(QColor(0, 0, 200));
                l_cf.setFontUnderline(true);
            }
        }
        p_builder.pushStyle(l_cf);
        // <q>: the quotation marks are not in the source text
        if(m_chunk_tag == "q") p_builder.insertSynthetic("“");
        p_builder.buildContent(this);
        if(m_chunk_tag == "q") p_builder.insertSynthetic("”");
        p_builder.popStyle();
    }

    if(l_relabel) p_builder.popAncestor();
}

/** --------------------------------------------------------------- TEI_TableInterp, TEI_RowInterp ---------------------------------
 * @brief QTextTable. A row opened on its own is shown as a one-row table
 */
void M1MidPlane::TEI_TableInterp::buildTextDoc(TEI_DocBuilder& p_builder){
    p_builder.beginBlockOwner(m_myself, QTextBlockFormat());
    // caption
    std::shared_ptr<Interp> l_heading = headingInterp();
    if(TEI_Container* l_hc = dynamic_cast<TEI_Container*>(l_heading.get())) p_builder.buildNode(l_hc);

    QList<TEI_Container*> l_rows;
    for(const auto& l_d : descendants())
        if(TEI_RowInterp* l_row = dynamic_cast<TEI_RowInterp*>(l_d.get())) l_rows.append(l_row);
    p_builder.buildTable(m_myself, l_rows);
    p_builder.endBlockOwner();
}

void M1MidPlane::TEI_RowInterp::buildTextDoc(TEI_DocBuilder& p_builder){
    // only reached as root: as part of a table, rows are handled by TEI_DocBuilder::buildTable()
    p_builder.buildTable(nullptr, QList<TEI_Container*>{this});
}

/** --------------------------------------------------------------- TEI_FigureInterp ---------------------------------
 * @brief centered image followed by its caption
 */
void M1MidPlane::TEI_FigureInterp::buildTextDoc(TEI_DocBuilder& p_builder){
    QTextBlockFormat l_bf;
    l_bf.setAlignment(Qt::AlignHCenter);
    l_bf.setBottomMargin(6);
    p_builder.beginBlockOwner(m_myself, l_bf);
    p_builder.insertImage(QUrl::fromPercentEncoding(m_myself->getField(M1Env::TEI_GRAPHIC_SIID).toUtf8()));
    p_builder.buildChildren(this);
    p_builder.endBlockOwner();
}

/** --------------------------------------------------------------- TEI_QuoteInterp ---------------------------------
 * @brief indented italic block(s)
 */
void M1MidPlane::TEI_QuoteInterp::buildTextDoc(TEI_DocBuilder& p_builder){
    QTextBlockFormat l_bf;
    l_bf.setLeftMargin(30);
    l_bf.setBottomMargin(6);
    QTextCharFormat l_cf;
    l_cf.setFontItalic(true);
    p_builder.beginBlockOwner(m_myself, l_bf, l_cf);
    p_builder.buildContent(this);
    p_builder.endBlockOwner();
}

/** --------------------------------------------------------------- TEI_ListInterp, TEI_ItemInterp ---------------------------------
 * @brief QTextList, one bullet per <item> (further blocks of an item are indented, without bullet)
 */
void M1MidPlane::TEI_ListInterp::buildTextDoc(TEI_DocBuilder& p_builder){
    QString l_type = m_myself->getField(M1Env::TEI_LIST_TYPE_SIID);
    p_builder.beginBlockOwner(m_myself, QTextBlockFormat());
    p_builder.beginList(m_myself, (l_type == "ordered" || l_type == "numbered") ? QTextListFormat::ListDecimal : QTextListFormat::ListDisc);
    p_builder.buildChildren(this);
    p_builder.endList();
    p_builder.endBlockOwner();
}

void M1MidPlane::TEI_ItemInterp::buildTextDoc(TEI_DocBuilder& p_builder){
    p_builder.beginBlockOwner(m_myself, QTextBlockFormat());
    p_builder.markListItem();
    p_builder.buildContent(this);
    p_builder.endBlockOwner();
}

#include "m1C_interp_tei_edit.moc"
