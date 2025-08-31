#include "m1C_interp.h"
#include "m1A_env.h"
#include "m1B_graph_init.h"
#include "m1B_store.h"
#include "m1B_lv2_item.h"

#include <QPainter>
#include <QResizeEvent>
#include <QRegularExpression>

// g_cat_interp_base
Q_LOGGING_CATEGORY(g_cat_interp_base, "interp.base")

QIcon M1MidPlane::Interp::cm_open;
QIcon M1MidPlane::Interp::cm_closed;


// ------------------------------------ Interp Base Class -----------------------------------------------------
void M1MidPlane::Interp::init(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Init Interp class members"))
    cm_open = QIcon("../Icons/Open.svg");
    Q_ASSERT_X( !cm_open.isNull(), "Interp::init()", "Open link Icon failed to load");
    cm_closed = QIcon("../Icons/Closed.svg");
    Q_ASSERT_X( !cm_closed.isNull(), "Interp::init()", "Closed link Icon failed to load");

    M1_FUNC_EXIT
}

M1MidPlane::Interp* M1MidPlane::Interp::getInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Getting appropriate Interp from: %1").arg(p_myself->dbgShort()))
    Interp* l_ret = nullptr;
    if(AutoInterp::wantIt(p_myself)){
        l_ret = new AutoInterp(p_myself, p_parent, p_depth);
        qCDebug(g_cat_interp_base) << "grabbed by AutoInterp()";
    }
    else if(TranslUnit::wantIt(p_myself))
        l_ret = new TranslUnit(p_myself, p_parent, p_depth);
    else if(FieldInterp::wantIt(p_myself))
        l_ret = new FieldInterp(p_myself, p_parent, p_depth);
    else if(SectionBeginEnd::wantIt(p_myself))
        l_ret = new SectionBeginEnd(p_myself, p_parent, p_depth);
    else if(SectionInterp::wantIt(p_myself))
        l_ret = new SectionInterp(p_myself, p_parent, p_depth);
    else if(UrlInterp::wantIt(p_myself))
        l_ret = new UrlInterp(p_myself, p_parent, p_depth);
    else if(BhashyaTranslation::wantIt(p_myself))
        l_ret = new BhashyaTranslation(p_myself, p_parent, p_depth);
    else if(TextInterp::wantIt(p_myself))
        l_ret = new TextInterp(p_myself, p_parent, p_depth);
    else if(TextOccurrence::wantIt(p_myself))
        l_ret = new TextOccurrence(p_myself, p_parent, p_depth);
    else if(SentenceInterp::wantIt(p_myself))
        l_ret = new SentenceInterp(p_myself, p_parent, p_depth);
    else if(BookInterp::wantIt(p_myself))
        l_ret = new BookInterp(p_myself, p_parent, p_depth);
    else{ // BookInterp
        qCDebug(g_cat_interp_base) << "default Interp()";
        l_ret = new Interp(p_myself, p_parent, p_depth);
    }
    M1_FUNC_EXIT
    return l_ret;
}

// ------------------------------------ Interp Base class -----------------------------------------------------
M1MidPlane::Interp::Interp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : QWidget::QWidget(p_parent){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Base Interp Constructor from: %1").arg(p_myself->dbgShort()))
    qCDebug(g_cat_interp_base) << "baseSize()  :" << this->baseSize();
    qCDebug(g_cat_interp_base) << "rect()      :" << this->rect();
    qCDebug(g_cat_interp_base) << "font height : " << this->fontMetrics().height();
    qCDebug(g_cat_interp_base) << "font family : " << this->fontInfo().family();
    m_depth = p_depth;
    m_myself = p_myself;
    m_target_height = (this->fontMetrics().height() * 1300) / 1000;
    m_target_beseline = (m_target_height * 800) / 1000;
    m_target_padding = (m_target_height - this->fontMetrics().height())/2;
    m_icon_size = this->fontMetrics().height();
    m_oc_x = m_target_padding + (p_depth + 1)*m_target_height;

    setMinimumHeight(m_target_height);
    setMaximumHeight(m_target_height);
    setFocusPolicy(Qt::StrongFocus);
    QPalette p = this->palette();
    p.setColor(QPalette::Window, QColor(Qt::darkRed));
    this->setPalette(p);
    this->setBackgroundRole(QPalette::Window);
    M1_FUNC_EXIT
}

void M1MidPlane::Interp::paintOC(QPainter& p){
    if(m_myself->flags() & M1Env::EDGE_IS_OPEN)
        M1MidPlane::Interp::cm_open.paint(&p, m_oc_x, m_target_padding, m_icon_size, m_icon_size);
    else
        M1MidPlane::Interp::cm_closed.paint(&p, m_oc_x, m_target_padding, m_icon_size, m_icon_size);
}

QString M1MidPlane::Interp::displayText(){
    return m_myself->getTarget_lv2()->text();
}

void M1MidPlane::Interp::paintEvent(QPaintEvent* p_event){
    qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " painting: " << p_event->rect();
    QPainter p(this);
    // QString l_text = m_myself->isFullEdge() ? m_myself->dbgHalf() : m_myself->dbgShort(0);
    M1Store::Storage::getQIcon(m_myself->getIconTypeMember())->paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    paintOC(p);
    M1Store::Storage::getQIcon(m_myself->getTarget_lv2()->getIconTypeMember())->paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), this->displayText());
}
void M1MidPlane::Interp::resizeEvent(QResizeEvent *p_event){
    qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " resized: " << p_event->size();
}

void M1MidPlane::Interp::mouseDoubleClickEvent(QMouseEvent *p_event){
    qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " double click: " << p_event->pos();
    emit gotoVertex(m_myself->getTarget_lv2());
}

QString base_html_fragment(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("base_html_fragment: %1").arg(p_myself->getTarget_lv2()->text()))
    QStringList l_edge_list;
    l_edge_list.append(QString("<tr><td>ID</td><td>:</td><td>%1</td></tr>\n").arg(p_myself->item_id()));
    l_edge_list.append(QString("<tr><td>Type</td><td>:</td><td>%1</td></tr>\n").arg(p_myself->dbgTypeShort()));
    QString l_edge_html = QString("<table>%1</table>\n").arg(l_edge_list.join("\n"));
    QString l_html;
    if((p_myself->flags() & M1Env::ITEM_NATURE_MASK) == M1Env::FULL_EDGE){
        QString l_target_html = p_myself->getTarget_lv2()->dbgStringHtml();
        qCDebug(g_cat_tree_display) << l_target_html;
        M1_FUNC_EXIT
            l_html = QString("<p style=\"font-weight: bold;\">Edge:</p>\n%1\n").arg(l_edge_html) +
               QString("<p style=\"font-weight: bold;\">Target:</p>\n<p>%1</p>\n").arg(l_target_html);
    }else{
        // Simple Edge
        QString l_payload_html = p_myself->text();
        qCDebug(g_cat_tree_display) << l_payload_html;
        M1_FUNC_EXIT
            l_html = QString("<p style=\"font-weight: bold;\">Edge:</p>\n%1").arg(l_edge_html) +
                QString("<p style=\"font-weight: bold;\">Payload: %1</p>\n").arg(l_payload_html);
    }
    return QString("<div style=\"font-family: 'Noto mono', Arial, sans-serif;\">%1</div>\n").arg(l_html);
}

QString M1MidPlane::Interp::getHtml(){
    return QString("<html>\n<Head></Head>\n<body>\n%1</body>\n</html>\n").arg(base_html_fragment(m_myself));
}

#include <QGraphicsSimpleTextItem>
#include <QGraphicsView>

QWidget *M1MidPlane::Interp::get_edit_widget(){
    qCDebug(g_cat_td_signals) << "Emitting <emitEdit> for" << m_myself->text();
    qCDebug(g_cat_td_signals) << QString("text: %1").arg(m_myself->dbgShort());

    QGraphicsScene* l_scene = new QGraphicsScene();

    QGraphicsSimpleTextItem* l_item = new QGraphicsSimpleTextItem(QString(m_myself->dbgShort()));
    static QFont f("Noto Mono", 12);
    l_item->setFont(f);
    l_scene->addItem(l_item);
    l_scene->setBackgroundBrush(Qt::white);

    return new QGraphicsView(l_scene);
}

void M1MidPlane::Interp::focusInEvent(QFocusEvent *p_event){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Interp::focusInEvent()"))
    qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " focus in: " << p_event->reason();
    qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " Background color: " << this->palette().color(QPalette::Window);

    emit emitHtml(getHtml());
    emit emitEdit(get_edit_widget());

    this->setAutoFillBackground(true);
    this->repaint();
    M1_FUNC_EXIT
}
void M1MidPlane::Interp::focusOutEvent(QFocusEvent *p_event){
    qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " focus out: " << p_event->reason();
    this->setAutoFillBackground(false);
    this->repaint();
}
void M1MidPlane::Interp::mousePressEvent(QMouseEvent *p_event){
    qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " mouse press: " << p_event->pos();
    if(m_oc_x < p_event->pos().x() && p_event->pos().x() < m_oc_x + m_target_height){
        qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " OC clicked";
        if(m_myself->flags() & M1Env::EDGE_IS_OPEN)
            m_myself->unSetFlag(M1Env::EDGE_IS_OPEN);
        else
            m_myself->setFlag(M1Env::EDGE_IS_OPEN);
        emit gotoVertex(nullptr);
    }
}

M1MidPlane::Interp::~Interp(){
    // qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " deleted";
}

// ------------------------------------ AutoInterp -----------------------------------------------------
bool M1MidPlane::AutoInterp::wantIt(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Getting appropriate Interp from: %1").arg(p_myself->dbgShort()))
    bool l_is_auto = p_myself->isOfType(M1Env::AUTO_SIID);
    qCDebug(g_cat_interp_base) << QString("p_myself->isOfType(M1Env::AUTO_SIID): %1").arg(l_is_auto ? "True" : "False");
    qCDebug(g_cat_interp_base) << QString("p_myself->isFullEdge()              : %1").arg(p_myself->isFullEdge() ? "True" : "False");
    M1_FUNC_EXIT
    return p_myself->isFullEdge() && p_myself->isOfType(M1Env::AUTO_SIID);
}
M1MidPlane::AutoInterp::AutoInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("AutoInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
void M1MidPlane::AutoInterp::paintEvent(QPaintEvent* p_event){
    QPainter p(this);
    // QString l_text = m_myself->getTarget_lv2()->text();
    M1Store::Storage::getQIcon(m_myself->getIconTypeMember())->paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    // M1MidPlane::Interp::cm_closed.paint(&p, m_target_padding + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    M1Store::Storage::getQIcon(m_myself->getTarget_lv2()->getIconTypeMember())->paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), this->displayText());
    p.setPen(Qt::darkGray);
    p.drawLine(QPoint(0,0), QPoint(this->width(),0));
    p.drawLine(QPoint(0,m_target_height-1), QPoint(this->width(),m_target_height-1));
}

// ------------------------------------ FieldInterp -----------------------------------------------------
bool M1MidPlane::FieldInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isSimpleEdge();
}
M1MidPlane::FieldInterp::FieldInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("FieldInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::FieldInterp::displayText(){
    return QString("[%1] ").arg(m_myself->dbgTypeShort()) + m_myself->text();
}

void M1MidPlane::FieldInterp::paintEvent(QPaintEvent* p_event){
    static QIcon ls_field_icon("../Icons/Field.svg");

    QPainter p(this);
    // QString l_text = QString("[%1] ").arg(m_myself->dbgTypeShort()) + m_myself->text();
    ls_field_icon.paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    // M1MidPlane::Interp::cm_closed.paint(&p, m_target_padding + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    M1Store::Storage::getQIcon(m_myself->getIconTypeMember())->paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), this->displayText());
}

// ------------------------------------ TranslUnit -----------------------------------------------------
QString g_html_template = "<html><Head></Head><body><div style=\"margin: 1em;\">\n%1</div></body></html>";
QString g_transl_template = "<tr><td style=\"font-family: 'Noto Sans', Arial, sans-serif; text-align: center;\">%1 (%2)</td></tr>\n";

QString tu_html_fragment(M1Store::Item_lv2* p_tu){
    qCDebug(g_cat_tree_display) << "p_tu         : " << p_tu->dbgShort(2);
    
    M1Store::Item_lv2* l_begin_word = p_tu->find_edge_edge(M1Store::TW_SECTION_2_OCC_BEGIN_SIID);
    M1Store::Item_lv2* l_end_word = p_tu->find_edge_edge(M1Store::TW_SECTION_2_OCC_END_SIID);

    if(l_begin_word != nullptr) l_begin_word = l_begin_word->getTarget_lv2();
    if(l_end_word != nullptr) l_end_word = l_end_word->getTarget_lv2();

    if(l_begin_word != nullptr) qCDebug(g_cat_tree_display) << "l_begin_word : " << l_begin_word->dbgShort(2);
    if(l_end_word != nullptr) qCDebug(g_cat_tree_display) << "l_end_word   : " << l_end_word->dbgShort(2);

    QString l_ground_text = "xx";
    QString l_translit = "yy";
    if(l_begin_word != nullptr){
        if(l_begin_word == l_end_word){
            l_ground_text = l_begin_word->getTarget_lv2()->text();
            l_translit = l_begin_word->getTarget_lv2()->getField(M1Store::TEXT_WORD_TRANSLIT_SIID);
        }
        else{
            l_ground_text = "";
            l_translit = "";
            for(M1Store::Item_lv2* l_occ = l_begin_word; true; l_occ = l_occ->get_next_lv2()){
                l_ground_text += QString("%1 ").arg(l_occ->getTarget_lv2()->text());
                l_translit += QString("%1 ").arg(l_occ->getTarget_lv2()->getField(M1Store::TEXT_WORD_TRANSLIT_SIID));
                if(l_occ == l_end_word) break;
            }
        }
    }
    l_ground_text = l_ground_text.trimmed();
    l_translit = l_translit.trimmed();

    QString l_translation_p = p_tu->getField(M1Store::TEXT_WFW_TRANSL_SIID, M1Store::TEXT_WFW_PRABUPADA_SIID);
    QString l_translation_s = p_tu->getField(M1Store::TEXT_WFW_TRANSL_SIID, M1Store::TEXT_WFW_SIVANANDA_SIID);
    return "<table style=\"display: inline-block; margin: 0 .5em 0 0;\">\n" +
        QString("<tr><td style=\"font-family: 'Noto Serif', 'Times New Roman', serif; text-align: center; color: maroon;\">%1</td></tr>\n").arg(l_ground_text) +
        QString("<tr><td style=\"font-style: italic; text-align: center;\">%1</td></tr>\n").arg(l_translit) +
       (l_translation_p.length() > 0 ? QString(g_transl_template).arg(l_translation_p).arg("P") : "") +
       (l_translation_s.length() > 0 ? QString(g_transl_template).arg(l_translation_s).arg("S") : "") +
        "</table>\n";
}

bool M1MidPlane::TranslUnit::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_WFW_UNIT_SIID);
}

M1MidPlane::TranslUnit::TranslUnit(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TranslUnit Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::TranslUnit::displayText(){
    return QString("%1 [%2]")
        .arg(m_myself->getTarget_lv2()->text())
        .arg(m_myself->getTarget_lv2()->getField(M1Store::TEXT_WFW_TRANSL_SIID, true));
}
QString M1MidPlane::TranslUnit::getHtml(){
    return g_html_template.arg(tu_html_fragment(m_myself->getTarget_lv2()));
}

// ------------------------------------ SectionBeginEnd -----------------------------------------------------
bool M1MidPlane::SectionBeginEnd::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() &&
           (p_myself->isOfType(M1Env::TW_SECTION_2_OCC_BEGIN_SIID) || p_myself->isOfType(M1Env::TW_SECTION_2_OCC_END_SIID));
}
M1MidPlane::SectionBeginEnd::SectionBeginEnd(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("SectionBeginEnd Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::SectionBeginEnd::displayText(){
    return QString("%1 [%2]")
        .arg(m_myself->getTarget_lv2()->getTarget_lv2()->text())
        .arg(m_myself->getTarget_lv2()->getTarget_lv2()->getField(M1Store::TEXT_WORD_TRANSLIT_SIID));
}
void M1MidPlane::SectionBeginEnd::paintEvent(QPaintEvent* p_event){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TranslUnit painting"))
    QPainter p(this);
    M1Store::Storage::getQIcon(m_myself->getIconTypeMember())->paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    paintOC(p);
    M1Store::Storage::getQIcon(m_myself->getTarget_lv2()->getTarget_lv2()->getIconTypeMember())->paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), this->displayText());
    M1_FUNC_EXIT
}
//------------------------------------ UrlInterp -----------------------------------------------------
bool M1MidPlane::UrlInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_URL_LINK_SIID);
}
QString M1MidPlane::UrlInterp::getHtml(){
    return m_url;
}
M1MidPlane::UrlInterp::UrlInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("UrlInterp Constructor from: %1").arg(p_myself->dbgShort()))

    m_label = m_myself->getTarget_lv2()->text();
    m_url = m_myself->getTarget_lv2()->getField(M1Env::TEXT_URL_LINK_SIID);
    M1_FUNC_EXIT
}

//------------------------------------ BhashyaTranslation -----------------------------------------------------
QString bt_html_fragment(M1Store::Item_lv2* p_bt){
    M1Store::Item_lv2* l_author_edge = p_bt->find_edge_edge(M1Store::TEXT_WRITTEN_BY_SIID);
    QString l_author = l_author_edge->getTarget_lv2()->text();
    QString l_text = p_bt->text();
    QString l_type = p_bt->isOfType(M1Env::TEXT_SLOKA_BHASHYA_SIID) ? "Bhashya" : "Translation";
    return QString("<div style=\"margin-bottom: 1em;\"><span style=\"font-weight: bold;\">%1</span>: ").arg(l_type) +
           QString("%1 <span style=\"font-style: italic; font-size: smaller; color: maroon;\">%2</span></div>").arg(l_text).arg(l_author);
}

bool M1MidPlane::BhashyaTranslation::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() &&
           (p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_SLOKA_BHASHYA_SIID) ||
                                      p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_SLOKA_TRANSLATION_SIID));
}
QString M1MidPlane::BhashyaTranslation::getHtml(){
    return g_html_template.arg(bt_html_fragment(m_myself->getTarget_lv2()));
}
QString M1MidPlane::BhashyaTranslation::displayText(){
    QString l_text = m_myself->getTarget_lv2()->text();
    if(l_text.length() > 100) l_text = l_text.left(100) + "...";
    return l_text;
}
M1MidPlane::BhashyaTranslation::BhashyaTranslation(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("UrlInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}

//------------------------------------ SectionInterp -----------------------------------------------------
QString si_html_fragment(M1Store::Item_lv2* p_si){
    QString l_html_wfw;
    QString l_html_lines;
    QString l_html_translations;
    QString l_html_bhashyas;
    for(M1Store::Item_lv2_iterator it = p_si->getIteratorTop(); !it.beyondEnd(); it.next()){
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_WFW_UNIT_SIID))
            l_html_wfw += tu_html_fragment(it.at()->getTarget_lv2());
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_SLOKA_LINE_SIID))
            l_html_lines += QString("<p style=\"margin: 0; padding: 0;\">%1</p>\n").arg(it.at()->getTarget_lv2()->text());
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_SLOKA_TRANSLATION_SIID))
            l_html_translations += bt_html_fragment(it.at()->getTarget_lv2());
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_SLOKA_BHASHYA_SIID))
            l_html_bhashyas += bt_html_fragment(it.at()->getTarget_lv2());
    }
    QString l_html = QString("<p style=\"font-weight: bold;\">%1</p>")
                         .arg(p_si->text()) +
                     QString("<div style=\"margin-bottom: 1em;\">%1</div>\n<div style=\"margin-bottom: 1em; background-color: SeaShell;\">%2</div>\n")
                         .arg(l_html_lines)
                         .arg(l_html_wfw) + l_html_translations + l_html_bhashyas;

    return l_html;
}
bool M1MidPlane::SectionInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_CHUNK_SIID);
}
QString M1MidPlane::SectionInterp::getHtml(){
    QString l_html = si_html_fragment(m_myself->getTarget_lv2());
    l_html += base_html_fragment(m_myself);
    return g_html_template.arg(l_html);
}
M1MidPlane::SectionInterp::SectionInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("SectionInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}

//------------------------------------ SentenceInterp -----------------------------------------------------
QString toText(M1Store::Item_lv2* p_occur_edge){
    QString l_text(p_occur_edge->getTarget_lv2()->text());
    if(p_occur_edge->getField(M1Store::CAPTL_SIID) == "true")
        l_text.front() = l_text.front().toUpper();
    return p_occur_edge->getField(M1Store::PCTLF_SIID) + l_text + p_occur_edge->getField(M1Store::PCTRT_SIID);
}
QString st_html_fragment(M1Store::Item_lv2* p_si){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("p_si: %1").arg(p_si->text()))
    QStringList l_word_list;
    for(M1Store::Item_lv2_iterator it = p_si->getIteratorTop(); !it.beyondEnd(); it.next()){
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::OCCUR_SIID)){
            QString l_text = toText(it.at()->getTarget_lv2());
            qCDebug(g_cat_interp_base) << "l_text" << l_text << it.at()->getTarget_lv2()->dbgShort();
            if(M1Store::Item_lv2* l_section = it.at()->getTarget_lv2()->find_edge(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID); l_section != nullptr){
                l_word_list.append(QString("<span style=\"font-size: smaller; color: maroon; font-weight: bold;\">%1</span> %2")
                                       .arg(l_section->getTarget_lv2()->text())
                                       .arg(l_text));
            }
            else{
                l_word_list.append(l_text);
            }
        }
    }
    M1_FUNC_EXIT
    return QString("<p>%1</p>").arg(l_word_list.join(" "));
}
bool M1MidPlane::SentenceInterp::wantIt(M1Store::Item_lv2* p_myself){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Is a sentence? %1")
                                         .arg(p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_SENTENCE_SIID) ? "true" : "false"))
    M1_FUNC_EXIT
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_SENTENCE_SIID);
}
M1MidPlane::SentenceInterp::SentenceInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : SectionInterp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("SentenceInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::SentenceInterp::getHtml(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Sent: %1").arg(m_myself->text()))
    QString l_html = st_html_fragment(m_myself->getTarget_lv2());
    l_html += base_html_fragment(m_myself);
    M1_FUNC_EXIT
    return g_html_template.arg(l_html);
}
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QTextEdit>
#include <QGraphicsProxyWidget>
#include <QVBoxLayout>
#include <QPushButton>

namespace M1UI{
// QGraphicsLayoutItem
class WordItem: public QGraphicsLayoutItem, public QGraphicsSimpleTextItem{
public:
    WordItem(const QString& p_txt, QGraphicsItem *p_parent = nullptr): QGraphicsSimpleTextItem(p_txt, p_parent), QGraphicsLayoutItem(){}
    virtual QSizeF sizeHint(Qt::SizeHint p_which, const QSizeF &p_constraint = QSizeF()) const {
        QSizeF l_ret = QGraphicsSimpleTextItem::boundingRect().size();
        qCDebug(g_cat_td_signals) << QString("WordItem::sizeHint [") << p_which << p_constraint << QString("] %1 -->").arg(this->text()) << l_ret;
        return l_ret;
    }
    virtual void setGeometry(const QRectF &p_rect){
        QGraphicsLayoutItem::setGeometry(p_rect);
        QGraphicsSimpleTextItem::setPos(p_rect.topLeft());
        qCDebug(g_cat_td_signals) << QString("WordItem::setGeometry") << p_rect << this->text();
    }
protected:
    virtual void paint(QPainter *p_painter,
                       const QStyleOptionGraphicsItem *p_option,
                       QWidget *p_widget){
        QGraphicsSimpleTextItem::paint(p_painter, p_option, p_widget);
        QRectF l_br = boundingRect();
        // p_painter->drawRect(l_br);
        l_br = QGraphicsLayoutItem::geometry();
        qCDebug(g_cat_td_signals) << QString("WordItem paint") <<
            this->text() << l_br.topLeft() << l_br.bottomRight() << QGraphicsLayoutItem::geometry();
    }
};

class StephanusItem: public WordItem{
public:
    StephanusItem(const QString& p_txt, QGraphicsItem *p_parent = nullptr): WordItem(p_txt, p_parent){}
protected:
    virtual void paint(QPainter *p_painter,
                       const QStyleOptionGraphicsItem *p_option,
                       QWidget *p_widget){

        WordItem::paint(p_painter, p_option, p_widget);
        p_painter->setPen(Qt::red);
        p_painter->drawRect(boundingRect());
    }
};

class FlowLayout: public QGraphicsLayout{
private:
    QList<WordItem*> m_item_list;
    QRectF m_outer_geometry;
    QGraphicsWidget* m_form;
    QString m_id_form;
    int m_margin = 20;
    int m_spacing = 10;
public:
    FlowLayout(const QString& p_id, QGraphicsWidget *p_parent = nullptr): QGraphicsLayout(p_parent){
        set_form(p_parent);
        m_id_form = p_id;
    }
    QRectF do_layout(const QRectF &p_rect){
        qCDebug(g_cat_td_signals) << QString("do_layout p_rect:") << m_id_form << p_rect;
        // QSizeF l_inner_size = QSizeF(0, 0);
        int l_init_x = p_rect.topLeft().x() + m_margin;
        int l_init_y = p_rect.topLeft().y() + m_margin;
        int l_x = l_init_x;
        int l_y = l_init_y;

        int max_x = 0;
        int max_y = 0;
        for(WordItem *l_item : std::as_const(m_item_list)){
            QSizeF l_size_hint = l_item->sizeHint(Qt::SizeHint::MinimumSize);
            qCDebug(g_cat_td_signals) << QString("SizeHint:") << l_item->text() << l_size_hint;

            if( l_x + l_size_hint.width() > p_rect.topRight().x() - m_margin){
                l_x = l_init_x;
                l_y += l_size_hint.height() + m_spacing;
            }

            if(l_x + l_size_hint.width() > max_x) max_x = l_x + l_size_hint.width();
            if(l_y + l_size_hint.height() > max_y) max_y = l_y + l_size_hint.height();

            // l_item->setGeometry(QRectF(QPointF(l_x, l_y), l_size_hint));
            l_item->setPos(l_x, l_y);
            qCDebug(g_cat_td_signals) << QString("item rec: (%1, %2) (%3, %4)")
                                             .arg(l_x).arg(l_y).arg(l_x + l_size_hint.width()-1).arg(l_y + l_size_hint.height()-1);

            /*
            int l_total_width = l_x - p_rect.topLeft().x() + l_size_hint.width() + m_margin;
            if(l_inner_size.width() < l_total_width) l_inner_size.setWidth(l_total_width);
            int l_total_height = l_y - p_rect.topLeft().y() + l_size_hint.height() + m_margin;
            if(l_inner_size.height () < l_total_height) l_inner_size.setHeight(l_total_height);
            */

            l_x += l_size_hint.width() + m_spacing;
        }
        // l_inner_size = QSizeF(max_x - l_init_x, l_last_item->geometry().bottom() - l_first_item->geometry().top());

        m_outer_geometry = QRectF(p_rect.topLeft(), QSizeF(p_rect.size().width(), max_y - l_init_y + 2*m_margin));
        qCDebug(g_cat_td_signals) << QString("do_layout m_outer_geometry:") << m_id_form << m_outer_geometry;
        return m_outer_geometry;
    }
    void set_form(QGraphicsWidget* p_form){m_form = p_form;}
    void addItem(WordItem* p_new_item){ m_item_list.append(p_new_item);}
    void setGeometry(const QRectF &p_rect){
        qCDebug(g_cat_td_signals) << QString("FlowLayout setGeometry:") << m_id_form << p_rect;
        QGraphicsLayout::setGeometry(p_rect);
    }
    virtual void widgetEvent(QEvent *p_event){
        qCDebug(g_cat_td_signals) << QString("FlowLayout widgetEvent:") << m_id_form << p_event;
        QGraphicsLayout::widgetEvent(p_event);
    }
    void updateGeometry(){
        qCDebug(g_cat_td_signals) << QString("FlowLayout updateGeometry:") << m_id_form;
        QGraphicsLayout::updateGeometry();
    }
    QSizeF sizeHint(Qt::SizeHint p_which, const QSizeF &p_constraint = QSizeF()) const{
        QSizeF l_sh = m_outer_geometry.size();
        qCDebug(g_cat_td_signals) << QString("FlowLayout::sizeHint [") << p_which << p_constraint << QString("] -->") << l_sh << m_id_form;
        return l_sh;
    }
    int count() const {
        qCDebug(g_cat_td_signals) << QString("Count:") << m_id_form << m_item_list.count();
        return m_item_list.count();
    }
    QGraphicsLayoutItem *itemAt(int p_index) const {
        Q_ASSERT_X( p_index >= 0 && p_index < count(), "FlowLayout::itemAt()", QString("p_index [%1] out of bounds").arg(p_index).toUtf8());
        qCDebug(g_cat_td_signals) << QString("itemAt:") << m_id_form << p_index << m_item_list.at(p_index)->text() << m_item_list.at(p_index)->geometry();
        return m_item_list.at(p_index);
    }
    void removeAt(int p_index){
        Q_ASSERT_X( p_index >= 0 && p_index < count(), "FlowLayout::removeAt()", QString("p_index [%1] out of bounds").arg(p_index).toUtf8());
        m_item_list.removeAt(p_index);
    }
};

class PassageEditor: public QGraphicsWidget{
private:
    FlowLayout* m_editor_layout;
    static QFont cm_font;
    QString m_id;
    QRectF m_outer_rect;
public:
    QString& id(){return m_id;}
    /*
    PassageEditor(M1Store::Item_lv2* p_sentence, const QString& p_id, QGraphicsWidget *p_parent): QGraphicsWidget(p_parent){
        qCDebug(g_cat_td_signals) << QString("PassageEditor") << p_id << p_sentence->text();
        m_editor_layout = new FlowLayout(p_id, this);
        this->setLayout(m_editor_layout);
        m_id = p_id;
        static_cast<QGraphicsLinearLayout*>(p_parent->layout())->addItem(this);

        //M1Store::Item_lv2* l_cur_occur = p_occur_start;
        for(M1Store::Item_lv2_iterator it = p_sentence->getIteratorTop(); !it.beyondEnd(); it.next()) {
            qCDebug(g_cat_td_signals) << QString("Current edge") << m_id << it.at()->dbgShort();
            if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::OCCUR_SIID)){
                QString l_text = toText(it.at()->getTarget_lv2());
                qCDebug(g_cat_td_signals) << QString("Adding word") << m_id << l_text;
                if(M1Store::Item_lv2* l_section = it.at()->getTarget_lv2()->find_edge(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID); l_section != nullptr){
                    // StephanusItem
                    M1UI::StephanusItem* l_steph_number_item = new M1UI::StephanusItem(l_section->getTarget_lv2()->text(), this);
                    l_steph_number_item->setFont(cm_font);
                    m_editor_layout->addItem(l_steph_number_item);
                }
                M1UI::WordItem* l_item = new M1UI::WordItem(l_text, this);
                l_item->setFont(cm_font);
                m_editor_layout->addItem(l_item);
            }
        }

        qCDebug(g_cat_td_signals) << QString("Count:") << m_id << m_editor_layout->count();
    }
    */
    // PassageEditor(M1Store::Item_lv2* p_sentence, const QString& p_id, QGraphicsWidget *p_parent): QGraphicsWidget(p_parent){
    PassageEditor(M1Store::Item_lv2* p_occur_start, const QString& p_id, QGraphicsWidget *p_parent=nullptr): QGraphicsWidget(p_parent){
        qCDebug(g_cat_td_signals) << QString("PassageEditor") << p_id << p_occur_start->text();
        m_editor_layout = new FlowLayout(p_id, this);
        this->setLayout(m_editor_layout);
        m_id = p_id;
        static_cast<QGraphicsLinearLayout*>(p_parent->layout())->addItem(this);

        M1Store::Item_lv2* l_cur_occur = p_occur_start;
        for(int l_count_words = 0; l_count_words < 100 && l_cur_occur->next_item_id() != M1Env::G_VOID_ITEM_ID; l_count_words++){
            qCDebug(g_cat_td_signals) << QString("Current edge") << m_id << l_cur_occur->dbgShort();
            if(l_cur_occur->isFullEdge() && l_cur_occur->isOfType(M1Env::OCCUR_SIID)){
                QString l_text = toText(l_cur_occur);
                qCDebug(g_cat_td_signals) << QString("Adding word") << m_id << l_text;
                if(M1Store::Item_lv2* l_section = l_cur_occur->find_edge(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID); l_section != nullptr){
                    // StephanusItem
                    M1UI::StephanusItem* l_steph_number_item = new M1UI::StephanusItem(l_section->getTarget_lv2()->text(), this);
                    l_steph_number_item->setFont(cm_font);
                    m_editor_layout->addItem(l_steph_number_item);
                }
                M1UI::WordItem* l_item = new M1UI::WordItem(l_text, this);
                l_item->setFont(cm_font);
                m_editor_layout->addItem(l_item);
            }

            l_cur_occur = l_cur_occur->get_next_lv2();
        }
        /*
        for(M1Store::Item_lv2_iterator it = p_sentence->getIteratorTop(); !it.beyondEnd(); it.next()) {
            qCDebug(g_cat_td_signals) << QString("Current edge") << m_id << it.at()->dbgShort();
            if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::OCCUR_SIID)){
                QString l_text = toText(it.at()->getTarget_lv2());
                qCDebug(g_cat_td_signals) << QString("Adding word") << m_id << l_text;
                if(M1Store::Item_lv2* l_section = it.at()->getTarget_lv2()->find_edge(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID); l_section != nullptr){
                    // StephanusItem
                    M1UI::StephanusItem* l_steph_number_item = new M1UI::StephanusItem(l_section->getTarget_lv2()->text(), this);
                    l_steph_number_item->setFont(cm_font);
                    m_editor_layout->addItem(l_steph_number_item);
                }
                M1UI::WordItem* l_item = new M1UI::WordItem(l_text, this);
                l_item->setFont(cm_font);
                m_editor_layout->addItem(l_item);
            }
        }*/

        qCDebug(g_cat_td_signals) << QString("Count:") << m_id << m_editor_layout->count();
    }
    virtual void paint(QPainter *p_painter,
                       const QStyleOptionGraphicsItem *p_option,
                       QWidget *p_widget){

        QGraphicsWidget::paint(p_painter, p_option, p_widget);

        /* QRectF l_contents = layout()->contentsRect();
        p_painter->setPen(Qt::green);
        p_painter->drawRect(l_contents);
        qCDebug(g_cat_td_signals) << QString("PassageEditor l_contents") << m_id << l_contents.topLeft() << l_contents.bottomRight();*/
        QRectF l_outline(QPointF(0, 0), size());
        p_painter->setPen(Qt::blue);
        p_painter->drawRect(l_outline);
        qCDebug(g_cat_td_signals) << QString("PassageEditor l_outline") << m_id << l_outline.topLeft() << l_outline.bottomRight();
    }
    virtual void setGeometry(const QRectF &p_rect){
        qCDebug(g_cat_td_signals) << QString("PassageEditor setGeometry()") << m_id << p_rect;
        QGraphicsWidget::setGeometry(p_rect);
    }
    virtual QVariant itemChange(GraphicsItemChange p_change, const QVariant &p_value){
        qCDebug(g_cat_td_signals) << QString("PassageEditor itemChange:") << m_id << p_change << p_value;
        return QGraphicsItem::itemChange(p_change, p_value);
    }
    virtual bool sceneEvent(QEvent *p_event){
        qCDebug(g_cat_td_signals) << QString("PassageEditor sceneEvent:") << m_id << p_event;
        return QGraphicsWidget::sceneEvent(p_event);
    }
    QSizeF sizeHint(Qt::SizeHint p_which, const QSizeF &p_constraint = QSizeF()) const {
        QSizeF l_sh = m_outer_rect.size();
        qCDebug(g_cat_td_signals) << QString("PassageEditor sizeHint:") << m_id << p_which << p_constraint << "-->" << l_sh;
        return l_sh;
    }
    void updateGeometry(){
        QGraphicsWidget::updateGeometry();
        qCDebug(g_cat_td_signals) << QString("PassageEditor updateGeometry:") << m_id << this->maximumWidth();
        // m_editor_layout->force_layout();
    }
    QRectF get_outer_rect(int p_view_width){
        m_outer_rect = m_editor_layout->do_layout(QRectF(QPointF(0, 0), QSizeF(p_view_width, 0)));
        qCDebug(g_cat_td_signals) << QString("PassageEditor get_outer_rect:") << m_id << m_outer_rect;
        return m_outer_rect;
    }
};

QFont PassageEditor::cm_font = QFont("Noto Mono", 12);

class VericalLayout: public QGraphicsLinearLayout{
private:
    int m_margin_vl = 10;
    QSizeF m_outer_size;
public:
    VericalLayout(QGraphicsLayoutItem *p_parent = nullptr): QGraphicsLinearLayout(Qt::Orientation::Vertical, p_parent){
        setContentsMargins(m_margin_vl, m_margin_vl, m_margin_vl, m_margin_vl);
        setSpacing(m_margin_vl);
    }
    virtual void setGeometry(const QRectF &p_rect) override {
        qCDebug(g_cat_td_signals) << QString("VericalLayout setGeometry:") << p_rect;
        // invalidate();
        QGraphicsLinearLayout::setGeometry(p_rect);
    }
    QSizeF sizeHint(Qt::SizeHint p_which, const QSizeF &p_constraint = QSizeF()) const {
        qCDebug(g_cat_td_signals) << QString("VericalLayout sizeHint:") << p_which << p_constraint << "-->" << m_outer_size;
        return m_outer_size;
    }
    virtual void widgetEvent(QEvent *p_event) override {
        qCDebug(g_cat_td_signals) << QString("VericalLayout widgetEvent:") << p_event << p_event->type();
        /*
        if(p_event->type() == QEvent::LayoutRequest || p_event->type() == QEvent::WindowActivate || p_event->type() == QEvent::GraphicsSceneResize){
            qCDebug(g_cat_td_signals) << QString("VericalLayout invalidate request:");
            // invalidate();
            // qCDebug(g_cat_td_signals) << QString("VericalLayout activate request:");
            // activate();
        }*/
        return QGraphicsLinearLayout::widgetEvent(p_event);
    }
    virtual void invalidate() override {
        qCDebug(g_cat_td_signals) << QString("VericalLayout invalidate:");
        // for(int i = 0; i < count(); i++) static_cast<QGraphicsWidget*>(itemAt(i))->layout()->invalidate();
        for(int i = 0; i < count(); i++) qCDebug(g_cat_td_signals) << static_cast<PassageEditor*>(itemAt(i))->id();
        // QGraphicsLinearLayout::invalidate();
    }
    virtual void updateGeometry() override {
        qCDebug(g_cat_td_signals) << QString("VericalLayout updateGeometry:");
        QGraphicsLinearLayout::updateGeometry();
    }
    QRectF get_outer_rect(int p_view_width){
        qCDebug(g_cat_td_signals) << QString("VericalLayout get_outer_rect:") << p_view_width;
        QRectF l_outer_rect;
        for(int i = 0; i < count(); i++){
            PassageEditor* l_pe = static_cast<PassageEditor*>(itemAt(i));
            QRectF l_rect = l_pe->get_outer_rect(p_view_width - 2*m_margin_vl);
            l_rect += QMarginsF(0, 0, 2*m_margin_vl, 0);
            qCDebug(g_cat_td_signals) << QString("VericalLayout pe:") << l_pe->id() << "--> outer rect:" << l_rect;
            if(l_outer_rect.isNull()) l_outer_rect = l_rect;
            else{
                l_outer_rect += QMarginsF(0, 0, 0, m_margin_vl);
                l_outer_rect = l_outer_rect.united(QRectF(QPointF(0, l_outer_rect.bottom()), l_rect.size()));
            }
        }
        l_outer_rect += QMarginsF(0, m_margin_vl, 0, m_margin_vl);
        m_outer_size = l_outer_rect.size();
        return l_outer_rect;
    }
};

class PassagesPanel: public QGraphicsWidget{
private:
    VericalLayout* m_outer_layout;
    int m_view_width = -1;
    QRectF m_outer_rect;
public:
    PassagesPanel(QGraphicsWidget *p_parent=nullptr): QGraphicsWidget(p_parent){
        m_outer_layout = new VericalLayout(this);
        this->setLayout(m_outer_layout);
    }
    virtual void paint(QPainter *p_painter,
                       const QStyleOptionGraphicsItem *p_option,
                       QWidget *p_widget) override{

        // QRectF l_contents = layout()->contentsRect();
        /*
        p_painter->setPen(Qt::GlobalColor::cyan);
        p_painter->drawRect(m_outer_rect);
        qCDebug(g_cat_td_signals) << QString("PassagesPanel m_outer_rect") << m_outer_rect.topLeft() << m_outer_rect.bottomRight(); */
        QRectF l_outline(QPointF(0, 0), size());
        p_painter->setPen(Qt::GlobalColor::darkYellow);
        p_painter->drawRect(l_outline);
        qCDebug(g_cat_td_signals) << QString("PassagesPanel l_outline") << l_outline.topLeft() << l_outline.bottomRight();
    }
    virtual void setGeometry(const QRectF &p_rect) override {
        qCDebug(g_cat_td_signals) << QString("PassagesPanel setGeometry:") << p_rect << " m_view_width:" << m_view_width;
        // m_outer_layout->invalidate();
        // m_outer_layout->activate();
        // m_outer_layout->updateGeometry();
        if(m_view_width != -1)
            QGraphicsWidget::setGeometry(p_rect);
    }
    void addLayout(QGraphicsWidget* p_widget){
        m_outer_layout->addItem(p_widget);
    }
    void add_stretch(){
        m_outer_layout->addStretch(1);
    }
    virtual bool event(QEvent *p_event) override{
        qCDebug(g_cat_td_signals) << QString("PassagesPanel event:") << p_event << " m_view_width:" << m_view_width;
        return QGraphicsWidget::event(p_event);
    }
    void updateGeometry() override {
        QGraphicsWidget::updateGeometry();
        qCDebug(g_cat_td_signals) << QString("PassagesPanel updateGeometry:") << " m_view_width:" << m_view_width;
    }
    QSizeF sizeHint(Qt::SizeHint p_which, const QSizeF &p_constraint = QSizeF()) const override {
        QSizeF l_sh = m_outer_rect.size();
        qCDebug(g_cat_td_signals) << QString("PassagesPanel sizeHint:") << p_which << p_constraint << "-->" << l_sh;
        return l_sh;
    }
    QRectF set_panel_width(int p_width){
        qCDebug(g_cat_td_signals) << QString("PassagesPanel set_view_width:") << p_width;
        m_view_width = p_width;
        m_outer_rect = m_outer_layout->get_outer_rect(p_width);
        return m_outer_rect;
    }
};

class Scene: public QGraphicsScene{
public:
    Scene():QGraphicsScene(){}
protected:
    virtual bool event(QEvent *p_event){
        qCDebug(g_cat_td_signals) << QString("Scene event:") << p_event;
        return QGraphicsScene::event(p_event);
    }
};

class View: public QGraphicsView{
private:
    PassagesPanel* m_outer_panel = nullptr;
    int m_margin_view = 10;
public:
    View(QGraphicsScene *p_scene):QGraphicsView(p_scene){}
    void set_panel(PassagesPanel* p_outer_panel){m_outer_panel = p_outer_panel;}
protected:
    virtual bool event(QEvent *p_event){
        qCDebug(g_cat_td_signals) << QString("View event:") << p_event;
        return QGraphicsView::event(p_event);
    }
    virtual void paintEvent(QPaintEvent *p_event){
        QRectF l_scene_rect = scene()->sceneRect();
        l_scene_rect = QRectF(l_scene_rect.topLeft(), QSize(l_scene_rect.size().width()-1, l_scene_rect.size().height()-1));
        qCDebug(g_cat_td_signals) << QString("View paintEvent l_scene_rect") << l_scene_rect.topLeft() << l_scene_rect.bottomRight();

        QGraphicsView::paintEvent(p_event);
    }
    virtual void resizeEvent(QResizeEvent *p_event){
        qCDebug(g_cat_td_signals) << QString("View resizeEvent p_event:") << p_event;

        // QRectF l_form_rect(QPoint(m_margin_view, m_margin_view), p_event->size().shrunkBy(QMargins(m_margin_view, m_margin_view, m_margin_view, m_margin_view)));
        // qCDebug(g_cat_td_signals) << QString("View resizeEvent l_form_rect:") << l_form_rect;

        qCDebug(g_cat_td_signals) << QString("View resizeEvent START") << "######################################### before set_view_width";
        QRectF l_panel_rect = m_outer_panel->set_panel_width(p_event->size().width() - 2*m_margin_view);

        int l_panel_size = l_panel_rect.size().height() + 2*m_margin_view;
        QRectF l_scene_rect = QRectF(QPoint(0, 0), QSizeF(p_event->size().width(), l_panel_size > p_event->size().height() ? l_panel_size : p_event->size().height()));
        scene()->setSceneRect(l_scene_rect);
        // qCDebug(g_cat_td_signals) << QString("View resizeEvent invalidate request:") << l_scene_rect << "+++++++++ ";
        // m_outer_panel->layout()->invalidate();
        m_outer_panel->setGeometry(QRectF(QPoint(m_margin_view, m_margin_view), l_panel_rect.size()));
        // qCDebug(g_cat_td_signals) << QString("View resizeEvent activate:") << l_scene_rect << "xxxxxxxxxx ";
        // m_outer_panel->layout()->activate();
        qCDebug(g_cat_td_signals) << QString("View resizeEvent l_scene_rect:") << l_scene_rect << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% after set_view_width";

        QGraphicsView::resizeEvent(p_event);
        qCDebug(g_cat_td_signals) << QString("View resizeEvent m_outer_panel->rect():") << p_event << m_outer_panel->rect() << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ END";
    }
};

}

QWidget *M1MidPlane::SentenceInterp::get_edit_widget(){
    // find previous Stephanus Number
    M1Store::Item_lv2* l_current_edge = m_myself->getTarget_lv2()->find_edge(M1Env::OWNS_SIID, M1Env::OCCUR_SIID)->getTarget_lv2();
    qCDebug(g_cat_td_signals) << QString("l_current_edge") << l_current_edge->dbgShort();
    M1Store::Item_lv2* l_section;
    while(true){
        qCDebug(g_cat_td_signals) << QString("l_current_edge") << l_current_edge->dbgShort();
        qCDebug(g_cat_td_signals) << QString("   previous") << l_current_edge->getPrevious_lv2()->dbgShort();
        qCDebug(g_cat_td_signals) << QString("   next    ") << l_current_edge->get_next_lv2()->dbgShort();
        l_section = l_current_edge->find_edge(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID);
        if(l_section != nullptr){
            l_section = l_section->getTarget_lv2();
            break;
        }
        l_current_edge = l_current_edge->getPrevious_lv2();
        // l_current_edge = l_current_edge->getNext_lv2();
    }
    qCDebug(g_cat_td_signals) << QString("Previous section") << l_section->dbgShort();
    M1Store::Item_lv2* l_greek_start = nullptr;
    M1Store::Item_lv2* l_jowett_start = nullptr;
    M1Store::Item_lv2* l_shorey_start = nullptr;
    for(M1Store::Item_lv2_iterator it = l_section->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::OCCUR_SIID)){
            qCDebug(g_cat_td_signals) << QString("section occur") << it.at()->getTarget_lv2()->dbgShort();
            if(it.at()->getTarget_lv2()->isOfType("RVGRK")) l_greek_start = it.at()->getTarget_lv2();
            if(it.at()->getTarget_lv2()->isOfType("RVJWT")) l_jowett_start = it.at()->getTarget_lv2();
            if(it.at()->getTarget_lv2()->isOfType("RVSHR")) l_shorey_start = it.at()->getTarget_lv2();
        }
    qCDebug(g_cat_td_signals) << QString("greek  start") << l_greek_start->dbgShort();
    qCDebug(g_cat_td_signals) << QString("jowett start") << l_jowett_start->dbgShort();
    qCDebug(g_cat_td_signals) << QString("shorey start") << l_shorey_start->dbgShort();

    // Set up UI
    QWidget* l_panel_widget = new QWidget();
    QVBoxLayout* l_panel_layout = new QVBoxLayout();
    l_panel_widget->setLayout(l_panel_layout);

    QWidget* l_button_bar = new QWidget(l_panel_widget);
    l_panel_layout->addWidget(l_button_bar);
    QHBoxLayout* l_bar_layout = new QHBoxLayout();
    l_button_bar->setLayout(l_bar_layout);

    QPushButton* l_btn1 = new QPushButton("Button 1", l_button_bar);
    QPushButton* l_btn2 = new QPushButton("Button 2", l_button_bar);

    qCDebug(g_cat_td_signals) << QString("l_btn1 sizePolicy") << l_btn1->sizePolicy();
    qCDebug(g_cat_td_signals) << QString("l_btn1 sizeHint") << l_btn1->sizeHint();

    l_bar_layout->addWidget(l_btn1);
    l_bar_layout->addWidget(l_btn2);
    l_bar_layout->addStretch(1);

    M1UI::Scene* l_scene = new M1UI::Scene();
    l_scene->setBackgroundBrush(Qt::white);

    M1UI::PassagesPanel* l_passages_panel = new M1UI::PassagesPanel();
    l_scene->addItem(l_passages_panel);

    // M1UI::PassageEditor *l_passage_editor_1 = new M1UI::PassageEditor(m_myself->getTarget_lv2());
    // M1UI::PassageEditor *l_passage_editor_1 = new M1UI::PassageEditor(l_shorey_start, "A", l_passages_panel);
    // M1UI::PassageEditor *l_passage_editor_2 = new M1UI::PassageEditor(l_jowett_start, "B", l_passages_panel);
    // M1UI::PassageEditor *l_passage_editor_1 = new M1UI::PassageEditor(m_myself->getTarget_lv2(), "A", l_passages_panel);
    // M1UI::PassageEditor *l_passage_editor_2 = new M1UI::PassageEditor(m_myself->getTarget_lv2(), "B", l_passages_panel);
    M1UI::PassageEditor *l_passage_editor_1 = new M1UI::PassageEditor(l_jowett_start, "A", l_passages_panel);
    M1UI::PassageEditor *l_passage_editor_2 = new M1UI::PassageEditor(l_shorey_start, "B", l_passages_panel);
    M1UI::PassageEditor *l_passage_editor_3 = new M1UI::PassageEditor(l_greek_start, "C", l_passages_panel);
    l_passages_panel->add_stretch();
    // M1UI::DummyLayout* l_layout = new M1UI::DummyLayout(Qt::Orientation::Vertical, l_form_item);
    // M1UI::FlowLayout* l_passage_layout = new M1UI::FlowLayout(l_passage_editor_1);
    //l_scene->addItem(l_passage_editor_1);
    //l_scene->addItem(l_passage_editor_2);
    l_passages_panel->add_stretch();

    M1UI::View* l_view = new M1UI::View(l_scene);
    l_view->set_panel(l_passages_panel);
    l_panel_layout->addWidget(l_view);

    return l_panel_widget;
}

//------------------------------------ BookInterp -----------------------------------------------------
bool M1MidPlane::BookInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_BOOK_SIID);
}
M1MidPlane::BookInterp::BookInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : SectionInterp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("BookInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString bk_html_fragment(M1Store::Item_lv2* p_si){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("p_si: %1").arg(p_si->text()))
    QStringList l_word_list;
    QString l_cur_version;
    for(M1Store::Item_lv2_iterator it = p_si->getIteratorTop(); !it.beyondEnd(); it.next()){
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_SENTENCE_SIID)){
            M1Store::Item_lv2* l_version = it.at()->getTarget_lv2()->find_edge(M1Env::ISA_SIID, M1Env::TXTVR_SIID, true);
            QString l_version_text = l_version->getTarget_lv2()->text();
            if(l_version_text != l_cur_version){
                l_cur_version = l_version_text;
                l_word_list.append(QString("<p style=\"border-top: 1px solid black; font-size: 60%;\">%1<p>").arg(l_version_text));
            }
            l_word_list.append(st_html_fragment(it.at()->getTarget_lv2()));
        }
    }
    M1_FUNC_EXIT
    return QString("<h3>%1</h3>").arg(p_si->text()) + l_word_list.join("\n");
}
QString M1MidPlane::BookInterp::getHtml(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Bk: %1").arg(m_myself->getTarget_lv2()->text()))
    QString l_html = bk_html_fragment(m_myself->getTarget_lv2());
    l_html += base_html_fragment(m_myself);
    M1_FUNC_EXIT
    return g_html_template.arg(l_html);
}
//------------------------------------ TextInterp -----------------------------------------------------
bool M1MidPlane::TextInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_SIID);
}
QString M1MidPlane::TextInterp::getHtml(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TextInterp::getHtml()"))
    QString l_html = QString("<h1>Title: %1</h1>\n").arg(m_myself->getTarget_lv2()->text());
    M1Store::Item_lv2* l_folder = m_myself->getTarget_lv2()->find_edge_target(M1Store::FOLDER_SIID);
    l_folder = l_folder->getTarget_lv2();
    for(M1Store::Item_lv2_iterator it = l_folder->getIteratorTop(); !it.beyondEnd(); it.next()){
        if(it.at()->getTarget_lv2()->isOfType(M1Store::TEXT_CHUNK_SIID))
            l_html += si_html_fragment(it.at()->getTarget_lv2());
    }
    l_html += base_html_fragment(m_myself);
    M1_FUNC_EXIT
    return g_html_template.arg(l_html);
}

M1MidPlane::TextInterp::TextInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TextInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}

//------------------------------------ TextOccurrence -----------------------------------------------------
bool M1MidPlane::TextOccurrence::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::OCCUR_SIID);
}
M1MidPlane::TextOccurrence::TextOccurrence(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TextOccurrence Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
QString M1MidPlane::TextOccurrence::displayText(){
    return toText(m_myself->getTarget_lv2());
}
void M1MidPlane::TextOccurrence::paintEvent(QPaintEvent* p_event){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TranslUnit painting"))
    QPainter p(this);

    M1Store::Storage::getQIcon(m_myself->getIconTypeMember())->paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    paintOC(p);
    M1Store::Storage::getQIcon(m_myself->getTarget_lv2()->getIconTypeMember())->paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), this->displayText());
    M1_FUNC_EXIT
}
