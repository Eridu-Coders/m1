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
QString M1MidPlane::SentenceInterp::occur_to_text(const M1Store::Item_lv2* p_occur_edge){
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
            QString l_text = M1MidPlane::SentenceInterp::occur_to_text(it.at()->getTarget_lv2());
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
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>

#include "m1D_passages_panel.h"

QWidget *M1MidPlane::SentenceInterp::get_edit_widget(){
    // find previous Stephanus Number
    M1Store::Item_lv2* l_current_edge = m_myself->getTarget_lv2()->find_edge(M1Env::OWNS_SIID, M1Env::OCCUR_SIID)->getTarget_lv2();
    qCDebug(g_cat_td_signals) << QString("l_current_edge") << l_current_edge->dbgShort();
    M1Store::Item_lv2* l_section;
    while(true){
        qCDebug(g_cat_td_signals) << QString("l_current_edge") << l_current_edge->dbgShort();
        qCDebug(g_cat_td_signals) << QString("   previous") << l_current_edge->get_previous_lv2()->dbgShort();
        qCDebug(g_cat_td_signals) << QString("   next    ") << l_current_edge->get_next_lv2()->dbgShort();
        l_section = l_current_edge->find_edge(M1Env::BLNGS_SIID, M1Env::STEPHANUS_SIID);
        if(l_section != nullptr){
            l_section = l_section->getTarget_lv2();
            break;
        }
        l_current_edge = l_current_edge->get_previous_lv2();
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

    QPushButton* l_btn0 = new QPushButton("< 10", l_button_bar);
    QPushButton* l_btn1 = new QPushButton("< 1", l_button_bar);
    QPushButton* l_btn2 = new QPushButton("> 1", l_button_bar);
    QPushButton* l_btn3 = new QPushButton("> 10", l_button_bar);
    QPushButton* l_btn4 = new QPushButton("Highlight", l_button_bar);
    l_btn4->setEnabled(false);

    QComboBox* l_cat_combo = new QComboBox(l_button_bar);
    // fill categories combo box
    M1Store::Item_lv2* l_republic = l_greek_start->getOrigin_lv2();
    M1Store::Item_lv2* l_cat_folder = l_republic->find_edge(M1Env::OWNS_SIID, M1Env::TEXT_HIGHLIGHT_CAT_FLDR_SIID)->getTarget_lv2();
    qCDebug(g_cat_td_signals) << QString("l_cat_folder") << l_cat_folder->dbgShort();

    QList<M1Store::Item_lv2*> l_cat_list;
    for(M1Store::Item_lv2_iterator it = l_cat_folder->getIteratorTop(); !it.beyondEnd(); it.next())
        if(it.at()->isFullEdge() && it.at()->getTarget_lv2()->isOfType(M1Env::TEXT_HIGHLIGHT_CAT_SIID)){
            M1Store::Item_lv2* l_cat = it.at()->getTarget_lv2();
            l_cat_list.append(l_cat);
            QString l_color = l_cat->getField(M1Env::HLCLR_SIID);
            qCDebug(g_cat_td_signals) << QString("Cat: ") << it.at()->dbgShort();
            qCDebug(g_cat_td_signals) << QString("l_color: ") << l_color;
            QPixmap l_color_pixmap(16, 16);
            l_color_pixmap.fill(QColor(l_color));
            l_cat_combo->addItem(QIcon(l_color_pixmap), l_cat->text());
        }

    // qCDebug(g_cat_td_signals) << QString("l_btn1 sizePolicy") << l_btn1->sizePolicy();
    // qCDebug(g_cat_td_signals) << QString("l_btn1 sizeHint") << l_btn1->sizeHint();

    l_bar_layout->addWidget(l_btn0);
    l_bar_layout->addWidget(l_btn1);
    l_bar_layout->addWidget(l_btn2);
    l_bar_layout->addWidget(l_btn3);
    l_bar_layout->addWidget(l_btn4);
    l_bar_layout->addWidget(l_cat_combo);
    l_bar_layout->addStretch(1);

    M1UI::Scene* l_scene = new M1UI::Scene();
    l_scene->setBackgroundBrush(Qt::white);

    M1UI::PassagesPanel* l_passages_panel = new M1UI::PassagesPanel(
        l_republic->find_edge(M1Env::OWNS_SIID, M1Env::TEXT_HIGHLIGHT_FLDR_SIID)->getTarget_lv2(),
        l_cat_list
    );
    l_scene->addItem(l_passages_panel);

    l_passages_panel->add_passage_editor(new M1UI::PassageEditor(l_jowett_start, "A", l_passages_panel));
    l_passages_panel->add_passage_editor(new M1UI::PassageEditor(l_shorey_start, "B", l_passages_panel));
    l_passages_panel->add_passage_editor(new M1UI::PassageEditor(l_greek_start, "C", l_passages_panel));

    QObject::connect(l_btn0, &QPushButton::clicked,
                     l_passages_panel, &M1UI::PassagesPanel::move_backwards_ten);
    QObject::connect(l_btn1, &QPushButton::clicked,
                     l_passages_panel, &M1UI::PassagesPanel::move_backwards_one);
    QObject::connect(l_btn2, &QPushButton::clicked,
                     l_passages_panel, &M1UI::PassagesPanel::move_forward_one);
    QObject::connect(l_btn3, &QPushButton::clicked,
                     l_passages_panel, &M1UI::PassagesPanel::move_forward_ten);
    QObject::connect(l_btn4, &QPushButton::clicked,
                     l_passages_panel, &M1UI::PassagesPanel::highlight);

    QObject::connect(l_cat_combo, &QComboBox::activated,
                     l_passages_panel, &M1UI::PassagesPanel::cat_select);

    // activate_highlight_button
    QObject::connect(l_passages_panel, &M1UI::PassagesPanel::activate_highlight_button,
                     l_btn4, &QPushButton::setEnabled);

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
    return M1MidPlane::SentenceInterp::occur_to_text(m_myself->getTarget_lv2());
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
