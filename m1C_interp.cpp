#include "m1C_interp.h"
#include "m1A_env.h"
#include "m1B_store.h"
#include "m1B_lv2_item.h"

#include <QPainter>
#include <QResizeEvent>

// g_cat_interp_base
Q_LOGGING_CATEGORY(g_cat_interp_base, "interp.base")

QIcon M1MidPlane::Interp::cm_open;
QIcon M1MidPlane::Interp::cm_closed;

/*
M1MidPlane::InterpStaticConstructor M1MidPlane::Interp::cm_the_init;

M1MidPlane::InterpStaticConstructor::InterpStaticConstructor(){
    printf("M1MidPlane::InterpStaticConstructor::InterpStaticConstructor()\n");
    M1MidPlane::Interp::cm_open = QIcon("../Icons/Open-inv.svg");
    M1MidPlane::Interp::cm_closed = QIcon("../Icons/Closed-inv.svg");
}
*/

// ------------------------------------ Interp Base Class -----------------------------------------------------
void M1MidPlane::Interp::init(){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Init Interp class members"))
    cm_open = QIcon("../Icons/Open.svg");
    cm_closed = QIcon("../Icons/Closed.svg");
    M1_FUNC_EXIT
}

M1MidPlane::Interp* M1MidPlane::Interp::getInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Getting appropriate Interp from: %1").arg(p_myself->dbgShort()))
    Interp* l_ret = nullptr;
    if(AutoInterp::wantIt(p_myself))
        l_ret = new AutoInterp(p_myself, p_parent, p_depth);
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
    else
        l_ret = new Interp(p_myself, p_parent, p_depth);
    M1_FUNC_EXIT
    return l_ret;
}

M1MidPlane::Interp::Interp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : QWidget::QWidget(p_parent){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("Base Interp Constructor from: %1").arg(p_myself->dbgShort()))
    qCDebug(g_cat_interp_base) << "baseSize()  :" << this->baseSize();
    qCDebug(g_cat_interp_base) << "rect()      :" << this->rect();
    qCDebug(g_cat_interp_base) << "font height : " << this->fontMetrics().height();
    qCDebug(g_cat_interp_base) << "font family : " << this->fontInfo().family();
    m_depth = p_depth;
    // m_active = false;
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

void M1MidPlane::Interp::paintEvent(QPaintEvent* p_event){
    qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " painting: " << p_event->rect();
    QPainter p(this);
    // QString l_text = m_myself->isFullEdge() ? m_myself->dbgHalf() : m_myself->dbgShort(0);
    QString l_text = m_myself->getTarget_lv2()->text();
    M1Store::Storage::getIcon(m_myself->getMaxTypeMember()).paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    paintOC(p);
    M1Store::Storage::getIcon(m_myself->getTarget_lv2()->getMaxTypeMember()).paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), l_text);
}
void M1MidPlane::Interp::resizeEvent(QResizeEvent *p_event){
    qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " resized: " << p_event->size();
}

void M1MidPlane::Interp::mouseDoubleClickEvent(QMouseEvent *p_event){
    qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " double click: " << p_event->pos();
    emit gotoVertex(m_myself->getTarget_lv2());
}

QString M1MidPlane::Interp::getHtml(){
    return QString("<html><Head></Head><body><p>%1</p></body></html>").arg(m_myself->dbgShort());
}

void M1MidPlane::Interp::focusInEvent(QFocusEvent *p_event){
    qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " focus in: " << p_event->reason();
    qCDebug(g_cat_tree_display) << m_myself->dbgShort() << " Background color: " << this->palette().color(QPalette::Window);
    QString l_html = getHtml();
    emit emitHtml(l_html);
    this->setAutoFillBackground(true);
    this->repaint();
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

// ------------------------------------ TranslUnit -----------------------------------------------------
QString g_html_template = "<html><Head></Head><body><div style=\"margin: 1em;\">\n%1</div></body></html>";
QString g_transl_template = "<tr><td style=\"font-family: 'Noto Sans', Arial, sans-serif; text-align: center;\">%1 (%2)</td></tr>\n";
/*
<table class="svInterlinearWord">
<tr><td class="svGroundGreek">l_ground</td>'</tr>
<tr><td class="svTranslit">l_translit</td></tr>
<tr><td><span class="TranslationText">l_translation</span></td></tr>
</table>

<table style="display: inline-block; margin: 0 .5em 0 0;">
<tr><td style="font-family: 'Noto Serif', 'Times New Roman', serif; text-align: center;">l_ground</td>'</tr>
<tr><td style="font-style: italic;">l_translit</td></tr>
<tr><td style="font-family: 'Noto Sans', Arial, sans-serif;">l_translation</td></tr>
</table>
*/
QString tu_html_fragment(M1Store::Item_lv2* p_tu){
    qCDebug(g_cat_tree_display) << "p_tu         : " << p_tu->dbgShort(2);

    M1Store::Item_lv2* l_begin_word = p_tu->find_edge2(M1Store::TW_SECTION_2_OCC_BEGIN_SIID);
    M1Store::Item_lv2* l_end_word = p_tu->find_edge2(M1Store::TW_SECTION_2_OCC_END_SIID);

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
            for(M1Store::Item_lv2* l_occ = l_begin_word; true; l_occ = l_occ->getNext_lv2()){
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
void M1MidPlane::TranslUnit::paintEvent(QPaintEvent* p_event){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TranslUnit painting"))
    QPainter p(this);
    QString l_text = QString("%1 [%2]")
                         .arg(m_myself->getTarget_lv2()->text())
                         .arg(m_myself->getTarget_lv2()->getField(M1Store::TEXT_WFW_TRANSL_SIID, true));
    M1Store::Storage::getIcon(m_myself->getMaxTypeMember()).paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    paintOC(p);
    M1Store::Storage::getIcon(m_myself->getTarget_lv2()->getMaxTypeMember()).paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), l_text);
    M1_FUNC_EXIT
}
QString M1MidPlane::TranslUnit::getHtml(){
    return g_html_template.arg(tu_html_fragment(m_myself->getTarget_lv2()));
}


// ------------------------------------ FieldInterp -----------------------------------------------------
bool M1MidPlane::FieldInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isSimpleEdge();
}
M1MidPlane::FieldInterp::FieldInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("FieldInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
void M1MidPlane::FieldInterp::paintEvent(QPaintEvent* p_event){
    static QIcon ls_field_icon("../Icons/Field.svg");

    QPainter p(this);
    QString l_text = QString("[%1] ").arg(m_myself->dbgTypeShort()) + m_myself->text();
    ls_field_icon.paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    // M1MidPlane::Interp::cm_closed.paint(&p, m_target_padding + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    M1Store::Storage::getIcon(m_myself->getMaxTypeMember()).paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), l_text);
}

// ------------------------------------ AutoInterp -----------------------------------------------------
bool M1MidPlane::AutoInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->isOfType(M1Env::AUTO_SPECIAL_ID);
}
M1MidPlane::AutoInterp::AutoInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("AutoInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
void M1MidPlane::AutoInterp::paintEvent(QPaintEvent* p_event){
    QPainter p(this);
    QString l_text = m_myself->getTarget_lv2()->text();
    M1Store::Storage::getIcon(m_myself->getMaxTypeMember()).paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    // M1MidPlane::Interp::cm_closed.paint(&p, m_target_padding + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    M1Store::Storage::getIcon(m_myself->getTarget_lv2()->getMaxTypeMember()).paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), l_text);
    p.setPen(Qt::darkGray);
    p.drawLine(QPoint(0,0), QPoint(this->width(),0));
    p.drawLine(QPoint(0,m_target_height-1), QPoint(this->width(),m_target_height-1));
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
void M1MidPlane::SectionBeginEnd::paintEvent(QPaintEvent* p_event){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TranslUnit painting"))
    QPainter p(this);
    QString l_text = QString("%1 [%2]")
                         .arg(m_myself->getTarget_lv2()->getTarget_lv2()->text())
                         .arg(m_myself->getTarget_lv2()->getTarget_lv2()->getField(M1Store::TEXT_WORD_TRANSLIT_SIID));;
    M1Store::Storage::getIcon(m_myself->getMaxTypeMember()).paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    paintOC(p);
    M1Store::Storage::getIcon(m_myself->getTarget_lv2()->getTarget_lv2()->getMaxTypeMember()).paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), l_text);
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

    m_label = QString(m_myself->getTarget_lv2()->text()).split("##")[0];
    m_url = QString(m_myself->getTarget_lv2()->text()).split("##")[1];
    M1_FUNC_EXIT
}
void M1MidPlane::UrlInterp::paintEvent(QPaintEvent* p_event){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("SectionInterp painting"))
    QPainter p(this);
    QString l_text = m_label;
    M1Store::Storage::getIcon(m_myself->getMaxTypeMember()).paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    paintOC(p);
    M1Store::Storage::getIcon(m_myself->getTarget_lv2()->getMaxTypeMember()).paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), l_text);
    M1_FUNC_EXIT
}

//------------------------------------ BhashyaTranslation -----------------------------------------------------
QString bt_html_fragment(M1Store::Item_lv2* p_bt){
    M1Store::Item_lv2* l_author_edge = p_bt->find_edge2(M1Store::TEXT_WRITTEN_BY_SIID);
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
M1MidPlane::BhashyaTranslation::BhashyaTranslation(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("UrlInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
void M1MidPlane::BhashyaTranslation::paintEvent(QPaintEvent* p_event){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("SectionInterp painting"))
    QPainter p(this);
    QString l_text = m_myself->getTarget_lv2()->text();
    M1Store::Storage::getIcon(m_myself->getMaxTypeMember()).paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    paintOC(p);
    M1Store::Storage::getIcon(m_myself->getTarget_lv2()->getMaxTypeMember()).paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), l_text);
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
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_SECTION_SPECIAL_ID);
}
QString M1MidPlane::SectionInterp::getHtml(){
    /*
    QString l_html_wfw;
    QString l_html_lines;
    QString l_html_translations;
    QString l_html_bhashyas;
    for(M1Store::Item_lv2_iterator it = m_myself->getTarget_lv2()->getIteratorTop(); !it.beyondEnd(); it.next()){
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
                         .arg(m_myself->getTarget_lv2()->text()) +
                    QString("<div style=\"margin-bottom: 1em;\">%1</div>\n<div style=\"margin-bottom: 1em;\">%2</div>\n")
                         .arg(l_html_lines)
                         .arg(l_html_wfw) + l_html_translations + l_html_bhashyas;
    */
    QString l_html = si_html_fragment(m_myself->getTarget_lv2());
    return g_html_template.arg(l_html);
}
M1MidPlane::SectionInterp::SectionInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("SectionInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
void M1MidPlane::SectionInterp::paintEvent(QPaintEvent* p_event){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("SectionInterp painting"))
    QPainter p(this);
    QString l_text = m_myself->getTarget_lv2()->text();
    M1Store::Storage::getIcon(m_myself->getMaxTypeMember()).paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    paintOC(p);
    M1Store::Storage::getIcon(m_myself->getTarget_lv2()->getMaxTypeMember()).paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), l_text);
    M1_FUNC_EXIT
}

//------------------------------------ TextInterp -----------------------------------------------------
bool M1MidPlane::TextInterp::wantIt(M1Store::Item_lv2* p_myself){
    return p_myself->isFullEdge() && p_myself->getTarget_lv2()->isOfType(M1Env::TEXT_SPECIAL_ID);
}
QString M1MidPlane::TextInterp::getHtml(){
    QString l_html;
    M1Store::Item_lv2* l_folder = m_myself->getTarget_lv2()->find_edge3(M1Store::FOLDER_SPECIAL_ID);
    l_folder = l_folder->getTarget_lv2();
    for(M1Store::Item_lv2_iterator it = l_folder->getIteratorTop(); !it.beyondEnd(); it.next()){
        if(it.at()->getTarget_lv2()->isOfType(M1Store::TEXT_SECTION_SPECIAL_ID))
            l_html += si_html_fragment(it.at()->getTarget_lv2());
    }
    return g_html_template.arg(l_html);
}
M1MidPlane::TextInterp::TextInterp(M1Store::Item_lv2* p_myself, QWidget* p_parent, int p_depth) : Interp(p_myself, p_parent, p_depth){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TextInterp Constructor from: %1").arg(p_myself->dbgShort()))
    M1_FUNC_EXIT
}
void M1MidPlane::TextInterp::paintEvent(QPaintEvent* p_event){
    M1_FUNC_ENTRY(g_cat_interp_base, QString("TextInterp painting"))
    QPainter p(this);
    QString l_text = m_myself->getTarget_lv2()->text();
    M1Store::Storage::getIcon(m_myself->getMaxTypeMember()).paint(&p, m_target_padding, m_target_padding, m_icon_size, m_icon_size);
    paintOC(p);
    M1Store::Storage::getIcon(m_myself->getTarget_lv2()->getMaxTypeMember()).paint(
        &p, m_oc_x + m_target_height, m_target_padding, m_icon_size, m_icon_size);
    p.setPen(Qt::white);
    p.drawText(QPoint(m_oc_x + m_target_height * 2, m_target_beseline), l_text);
    M1_FUNC_EXIT
}



