#include "m1D_fields_panel.h"
#include "m1B_store.h"
#include "m1B_lv2_iterators.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLoggingCategory>
#include "m1B_archive.h"

Q_LOGGING_CATEGORY(g_cat_fields_panel, "fields_panel")

/**
 * @brief Constructor - builds the static part of the panel (title, scroll area for the form, add/save/revert controls)
 * @param p_parent parent widget
 */
M1UI::FieldsPanel::FieldsPanel(QWidget* p_parent) : QWidget(p_parent){
    QVBoxLayout* l_layout = new QVBoxLayout(this);

    // vertex label editor
    QHBoxLayout* l_label_layout = new QHBoxLayout();
    l_label_layout->addWidget(new QLabel("Label", this));
    m_label_edit = new QLineEdit(this);
    QFont l_label_font = m_label_edit->font();
    l_label_font.setBold(true);
    m_label_edit->setFont(l_label_font);
    QObject::connect(m_label_edit, &QLineEdit::textEdited, this, [this](){ setDirty(true); });
    l_label_layout->addWidget(m_label_edit, 1);
    l_layout->addLayout(l_label_layout);

    // the form itself lives in a scroll area (re-created by rebuildForm())
    m_scroll = new QScrollArea(this);
    m_scroll->setWidgetResizable(true);
    l_layout->addWidget(m_scroll, 1);

    // new field controls: type combo (same text as the main window type combos, but no icon - field types have none) + Add button
    QHBoxLayout* l_add_layout = new QHBoxLayout();
    m_type_combo = new QComboBox(this);
    for(const M1Store::SpecialItem* l_special : M1Store::StorageStatic::getFieldTypes())
        m_type_combo->addItem(l_special->mnemonicAndLabel(),
                              QVariant(static_cast<uint>(l_special->specialId())));
    l_add_layout->addWidget(m_type_combo, 1);
    m_add_btn = new QPushButton("Add", this);
    l_add_layout->addWidget(m_add_btn);
    l_layout->addLayout(l_add_layout);

    // save / revert
    QHBoxLayout* l_save_layout = new QHBoxLayout();
    m_save_btn = new QPushButton("Save", this);
    m_revert_btn = new QPushButton("Revert", this);
    l_save_layout->addWidget(m_save_btn);
    l_save_layout->addWidget(m_revert_btn);
    l_save_layout->addStretch(1);
    l_layout->addLayout(l_save_layout);

    m_message = new QLabel(this);
    m_message->setTextFormat(Qt::PlainText);
    m_message->setWordWrap(true);
    l_layout->addWidget(m_message);

    QObject::connect(m_add_btn, &QPushButton::clicked, this, &M1UI::FieldsPanel::addField);
    QObject::connect(m_save_btn, &QPushButton::clicked, this, &M1UI::FieldsPanel::save);
    QObject::connect(m_revert_btn, &QPushButton::clicked, this, &M1UI::FieldsPanel::revert);

    rebuildForm();
}

/**
 * @brief the field type of an edge: first of its special types with SI_FIELD_TYPE set, otherwise first special type
 * @param p_edge the edge
 * @return the type special item, or nullptr if none (type stored as an ItemID or no type at all)
 */
const M1Store::SpecialItem* M1UI::FieldsPanel::fieldTypeOf(const M1Store::Item_lv2* p_edge){
    if(p_edge->flags() & M1Env::TYPE_IS_ITEM_ID) return nullptr;

    const M1Store::SpecialItem* l_first = nullptr;
    for(unsigned int i = 0; i < 4; i++){
        M1Env::SpecialItemID l_si_id = p_edge->getType_si_id(i);
        if(l_si_id == M1Env::G_VOID_SI_ID) continue;
        const M1Store::SpecialItem* l_si = M1Store::StorageStatic::getSpecialItemPointer(l_si_id);
        if(l_si->flags() & M1Env::SI_FIELD_TYPE) return l_si;
        if(l_first == nullptr) l_first = l_si;
    }
    return l_first;
}

/**
 * @brief is this edge a field? (simple edge, or full edge leading to a simple vertex)
 * @param p_edge the edge
 * @return true if it is
 */
bool M1UI::FieldsPanel::isFieldEdge(const M1Store::Item_lv2* p_edge){
    if(p_edge->isDead()) return false;
    if(p_edge->isSimpleEdge()) return true;
    return p_edge->isFullEdge() && p_edge->getTarget_lv2()->isSimpleVertex();
}

/**
 * @brief [slot] sets the vertex whose fields are edited (unsaved edits on the previous vertex are discarded)
 * @param p_vertex the vertex (nullptr or a non full vertex --> empty, disabled form)
 */
void M1UI::FieldsPanel::setVertex(M1Store::Item_lv2* p_vertex){
    qCDebug(g_cat_fields_panel) << "setVertex" << (p_vertex == nullptr ? QString("nullptr") : p_vertex->dbgShort());
    m_vertex = (p_vertex != nullptr && p_vertex->isFullVertex()) ? p_vertex : nullptr;
    m_message->clear();
    rebuildForm();
}

/**
 * @brief remove all rows (the form widget is replaced by an empty one)
 */
void M1UI::FieldsPanel::clearForm(){
    m_rows.clear();
    m_form_widget = new QWidget();
    m_form_layout = new QGridLayout(m_form_widget);
    m_form_layout->setColumnStretch(1, 1);
    // setWidget() destroys the previous form widget (and all its editors)
    m_scroll->setWidget(m_form_widget);
}

/**
 * @brief add one row to the form (and to m_rows)
 * @param p_row row data (m_editor is created here)
 * @param p_type_text text of the type label (left column)
 */
void M1UI::FieldsPanel::addFormRow(const FieldRow& p_row, const QString& p_type_text){
    FieldRow l_row = p_row;
    const int l_grid_row = m_rows.count();

    QLabel* l_type_label = new QLabel(p_type_text, m_form_widget);
    l_type_label->setTextFormat(Qt::PlainText);
    l_row.m_editor = new QLineEdit(l_row.m_original, m_form_widget);
    l_row.m_editor->setPlaceholderText(l_row.m_edge == nullptr ? "new field value" : "(empty --> field removed on Save)");
    QObject::connect(l_row.m_editor, &QLineEdit::textEdited, this, [this](){ setDirty(true); });

    // the stretch row is always the one after the last field row
    m_form_layout->setRowStretch(l_grid_row, 0);
    m_form_layout->addWidget(l_type_label, l_grid_row, 0);
    m_form_layout->addWidget(l_row.m_editor, l_grid_row, 1);
    m_form_layout->setRowStretch(l_grid_row + 1, 1);

    m_rows.append(l_row);
}

/**
 * @brief re-create the form from the store contents
 */
void M1UI::FieldsPanel::rebuildForm(){
    clearForm();

    const bool l_enabled = m_vertex != nullptr;
    m_type_combo->setEnabled(l_enabled);
    m_add_btn->setEnabled(l_enabled);
    m_label_edit->setEnabled(l_enabled);

    if(m_vertex == nullptr){
        m_label_original.clear();
        m_label_edit->clear();
        m_label_edit->setPlaceholderText("No node selected");
        setDirty(false);
        return;
    }
    m_label_original = QString::fromUtf8(m_vertex->text());
    m_label_edit->setText(m_label_original);
    m_label_edit->setPlaceholderText("vertex label");

    for(M1Store::Item_lv2_iterator l_it = m_vertex->getIteratorTop(); !l_it.beyondEnd(); l_it.next()){
        M1Store::Item_lv2* l_edge = l_it.at();
        if(!isFieldEdge(l_edge)) continue;

        FieldRow l_row;
        l_row.m_edge = l_edge;
        const M1Store::SpecialItem* l_type = fieldTypeOf(l_edge);
        l_row.m_type = (l_type == nullptr) ? M1Env::G_VOID_SI_ID : l_type->specialId();
        l_row.m_original = QString::fromUtf8(l_edge->isSimpleEdge() ? l_edge->text() : l_edge->getTarget_lv2()->text());
        addFormRow(l_row, l_type == nullptr ? QString("?") : l_type->mnemonicAndLabel());
    }
    if(m_rows.isEmpty()) m_message->setText("This node has no fields");

    setDirty(false);
}

/**
 * @brief Save/Revert enabled only when there is something to save
 */
void M1UI::FieldsPanel::setDirty(bool p_dirty){
    m_save_btn->setEnabled(p_dirty);
    m_revert_btn->setEnabled(p_dirty);
}

/**
 * @brief [slot] add an empty row for a new field of the type selected in the combo (created in the store on Save)
 */
void M1UI::FieldsPanel::addField(){
    if(m_vertex == nullptr || m_type_combo->currentIndex() < 0) return;

    const M1Env::SpecialItemID l_si_id = static_cast<M1Env::SpecialItemID>(m_type_combo->currentData().toUInt());
    FieldRow l_row;
    l_row.m_type = l_si_id;
    addFormRow(l_row, M1Store::StorageStatic::getSpecialItemPointer(l_si_id)->mnemonicAndLabel() + " (new)");
    m_message->clear();
    m_rows.last().m_editor->setFocus();
    setDirty(true);
}

/**
 * @brief [slot] write changes to the store
 *
 * - label modified --> vertex text updated (must not be empty)
 * - new rows with a non-empty value --> new simple edge field (Item_lv2::setFieldEdgeForceNew())
 * - existing rows emptied --> field removed (Item_lv2::unlinkEdge())
 * - existing rows modified --> text of the simple edge / simple vertex updated
 *
 * All values are checked first (simple vertex text length) so that nothing is written if one of them is invalid.
 */
void M1UI::FieldsPanel::save(){
    if(m_vertex == nullptr) return;

    // validation pass
    if(m_label_edit->text().trimmed().isEmpty()){
        m_message->setText("Not saved: the label cannot be empty");
        m_label_edit->setFocus();
        return;
    }
    for(const FieldRow& l_row : std::as_const(m_rows)){
        const QString l_value = l_row.m_editor->text();
        if(l_row.m_edge != nullptr && l_row.m_edge->isFullEdge() && l_value != l_row.m_original &&
            l_value.toUtf8().length() > static_cast<qsizetype>(M1Env::SIMPLE_VERTEX_TEXT_LEN - 1)){
            m_message->setText(QString("Not saved: value \"%1\" is too long (max %2 UTF-8 bytes for this field)")
                                   .arg(l_value).arg(M1Env::SIMPLE_VERTEX_TEXT_LEN - 1));
            l_row.m_editor->setFocus();
            return;
        }
    }

    // write pass
    M1Store::Transaction l_archive_tx("Save fields");
    int l_changes = 0;
    if(m_label_edit->text() != m_label_original){
        qCDebug(g_cat_fields_panel) << "new label" << m_label_edit->text();
        m_vertex->setText_lv1(m_label_edit->text());
        l_changes++;
    }
    for(const FieldRow& l_row : std::as_const(m_rows)){
        const QString l_value = l_row.m_editor->text();
        if(l_row.m_edge == nullptr){
            if(l_value.isEmpty() || l_row.m_type == M1Env::G_VOID_SI_ID) continue;
            qCDebug(g_cat_fields_panel) << "new field" << l_row.m_type << l_value;
            m_vertex->setFieldEdgeForceNew(l_value, l_row.m_type);
        }
        else if(l_value == l_row.m_original) continue;
        else if(l_value.isEmpty()){
            qCDebug(g_cat_fields_panel) << "removing field" << l_row.m_edge->dbgShort();
            l_row.m_edge->unlinkEdge();
        }
        else if(l_row.m_edge->isSimpleEdge())
            l_row.m_edge->setText_lv1(l_value);
        else
            l_row.m_edge->getTarget_lv2()->setText_lv1(l_value);
        l_changes++;
    }

    M1Store::Item_lv2* l_vertex = m_vertex;
    rebuildForm();
    m_message->setText(l_changes == 0 ? QString("Nothing to save") : QString("%1 change(s) saved").arg(l_changes));
    if(l_changes > 0) emit fieldsSaved(l_vertex);
}

/**
 * @brief [slot] discard unsaved edits
 */
void M1UI::FieldsPanel::revert(){
    rebuildForm();
    m_message->setText("Changes discarded");
}
