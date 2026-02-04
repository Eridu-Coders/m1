#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QRegularExpression>

#include <iostream>

#include "m1A_env.h"
#include "m1A_constants.h"

// default inits for static members
bool M1Env::EnvStatic::cm_block = false;
QTextStream* M1Env::EnvStatic::cm_debug_stream = 0;
QFile* M1Env::EnvStatic::cm_debug_file = 0;
QString M1Env::EnvStatic::cm_normal_filter;
/// stack to contain the nested categories of corresponding entry()/exit() pairs
std::stack<QString> M1Env::EnvStatic::cm_current_category;
int M1Env::EnvStatic::cm_depth = 0;
QStringList M1Env::EnvStatic::cm_list_excluded_catergory_for_screen;

QRegularExpression g_re_space(R"(\s+)");
QRegularExpression g_re_punc(R"(^(\W*)(\w+)(\W*))");
QRegularExpression g_re_tags(R"(<[^>]+>)");
QRegularExpression g_re_initial_p(R"(^\s*<p>\s*)", QRegularExpression::DotMatchesEverythingOption);
QRegularExpression g_re_final_p(R"(\s*<\/p>\s*$)", QRegularExpression::DotMatchesEverythingOption);
QRegularExpression g_re_initial_table(R"(^\s*<table>\s*)", QRegularExpression::DotMatchesEverythingOption);
QRegularExpression g_re_final_table(R"(\s*<\/table>\s*$)", QRegularExpression::DotMatchesEverythingOption);
QRegularExpression g_re_capture_gammar(R"(<div\s+class=\"+grammar\"+>(.*)<\/div>)", QRegularExpression::DotMatchesEverythingOption);
QRegularExpression g_re_cap_initial(R"(^[A-Z])");
QRegularExpression g_re_sent_num(R"(\d+$)");
QRegularExpression g_re_stephanus(R"((\d+[a-e])<\/span>)");
QRegularExpression g_re_no_base(R"(^Base:\s+)");
QRegularExpression g_re_extract_vt(R"(^\[([^]]+)\]\d*[a-e]?\s*(.*)$)");
QRegularExpression g_re_html_tags(R"(<[^>]+>)");

/**
 * @brief M1Store::M1Env::myMessageHandler message handler callback for Qt debug infra
 * @param p_type Qt log level (Debug/Warning/Critical/Fatal)
 * @param p_context function name, file, line, etc
 * @param p_msg the error message
 */
void M1Env::EnvStatic::myMessageHandler(QtMsgType p_type, const QMessageLogContext &p_context, const QString & p_msg){
    if( M1Env::EnvStatic::cm_block ) return;

    sendToLogChannels(p_type,
                      p_msg,
                      p_context.category,
                      p_context.file,
                      p_context.line,
                      p_context.function);
}

/**
 * @brief M1Env::EnvStatic::sendToLogChannels
 * @param p_type
 * @param p_msg
 * @param p_cat
 * @param p_file
 * @param p_line
 * @param p_func_deco
 */
void M1Env::EnvStatic::sendToLogChannels(QtMsgType p_type,
                                           const QString & p_msg,
                                           const QString & p_cat,
                                           const QString& p_file,
                                           int p_line,
                                           const QString& p_func_deco){

    QString l_level;
    if(p_type == QtDebugMsg) l_level = "Debug";
    else if(p_type == QtWarningMsg) l_level = "Warning";
    else if(p_type == QtCriticalMsg) l_level = "Critical";
    else if(p_type == QtFatalMsg) l_level = "Fatal";

    // remove double quotes from message if any (more double quotes will be added below)
    QString l_msg = (p_msg.at(0) == '"' && p_msg.at(p_msg.length()-1) == '"') ? p_msg.sliced(1, p_msg.length()-2) : p_msg;

    // CSV message
    l_msg.replace("\"", "\"\""); // double quote escape
    QString l_txt_csv = QString("\"%1\";\"%2\";\"%3\";\"%4\";\"%5\";\"%6\";\"%7\"")
                            .arg(l_level, p_cat, QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz"))
                            .arg(p_func_deco, indentSpace() + l_msg)
                            //.arg(p_context.function, indentSpace() + l_msg + QString(" [cm_depth = %1]").arg(cm_depth))
                            .arg(p_file).arg(p_line);

    // CSV message to stream
    (*cm_debug_stream) << l_txt_csv << Qt::endl << Qt::flush;

    if(! cm_list_excluded_catergory_for_screen.contains(p_cat)){
        // message for screen debug
        QString l_txt_screen = QString("[%1] [%2] %3 (%4:%5) %6")
                                   .arg(l_level, p_cat, QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                                   .arg(p_func_deco)
                                   .arg(p_line)
                                   .arg(l_msg);
        // screen message to screen
        std::cout << l_txt_screen.toStdString() << std::endl << std::flush;
    }
}

void M1Env::EnvStatic::addExcludeCatergoryForScreen(QString p_cat){
    cm_list_excluded_catergory_for_screen.append(p_cat);
}

QString M1Env::EnvStatic::indentSpace(){
    return QString("...").repeated(cm_depth) + QString("%1").arg(cm_depth, 2);
}

/**
 * @brief M1Env::EnvStatic::entry
 * @param p_log_cat
 * @param p_msg
 * @param p_file
 * @param p_line
 * @param p_func
 */
void M1Env::EnvStatic::entry(const QLoggingCategory& p_log_cat(),
                               const QString& p_msg,
                               const QString& p_file,
                               int p_line,
                               const QString& p_func_deco,
                               const QString& p_func){

    if(p_log_cat().isDebugEnabled()){
        cm_current_category.push(p_log_cat().categoryName());
        sendToLogChannels(QtDebugMsg,
                          QString("[Entering %1()] ").arg(p_func) + p_msg,
                          cm_current_category.top(),
                          p_file,
                          p_line,
                          p_func_deco);
        cm_depth += 1;
    }
    // current category not enabled --> push empty string placeholder
    else cm_current_category.push("");
}

/**
 * @brief M1Env::EnvStatic::exit
 * @param p_file
 * @param p_line
 * @param p_func
 */
void M1Env::EnvStatic::exit(
    const QString& p_file,
    int p_line,
    const QString& p_func_deco,
    const QString& p_func){

    Q_ASSERT_X(!cm_current_category.empty(),
               "M1Env::M1EnvStatic::exit(const QString& p_file, ...",
               "cm_current_category should not be empty");
    if(cm_current_category.top().length() > 0){
        // the matching entry() had an enabled category name
        cm_depth -= 1;
        sendToLogChannels(QtDebugMsg,
                          QString("[Exiting %1()]").arg(p_func),
                          cm_current_category.top(),
                          p_file,
                          p_line,
                          p_func_deco);
    }
    cm_current_category.pop();
}

/**
 * @brief M1Store::M1Env::init init m1 env
 */
void M1Env::EnvStatic::init(){
    // open stream for CSV debug messages
    cm_debug_file = new QFile(LOG_FILE_NAME);
    cm_debug_file->open(QIODevice::WriteOnly);
    cm_debug_stream = new QTextStream(cm_debug_file);

    // declare the message handler callback to Qt Log
    qInstallMessageHandler(myMessageHandler); // Install the handler

    cm_block = false;
    // initial debug message
    qDebug() << "Logger set-up complete";
}

/**
 * @brief M1Store::M1Env::close close m1 env
 */
void M1Env::EnvStatic::close(){
    // last debug message
    qDebug() << "Closing Logging system";

    // flush and close debug stream
    cm_debug_stream->flush();
    delete cm_debug_stream;
    delete cm_debug_file;
    cm_block = true;
}

void M1Env::EnvStatic::setNormalFilter(const QString& p_filter){
    cm_normal_filter = p_filter;
    setSilentMode(false);
}

void M1Env::EnvStatic::setTmpDebug(bool p_on){
    // "tmp.debug=false\n"
    qCDebug(g_cat_tmp_debug) << "p_on START:" << (p_on ? "true" : "false");
    QString l_filter = cm_normal_filter;
    if(p_on)
        l_filter += "tmp_debug=true\n";
    else
        l_filter += "tmp_debug=false\n";
    qCDebug(g_cat_tmp_debug).noquote() << "l_filter:" << l_filter;
    QLoggingCategory::setFilterRules(l_filter);
    qCDebug(g_cat_tmp_debug) << "p_on END:" << (p_on ? "true" : "false");
}

void M1Env::EnvStatic::setSilentMode(bool p_set){
    if(p_set)
        // set debug categories to just allow g_cat_silence (= "dump")
        QLoggingCategory::setFilterRules("*.debug=false\n"
                                         "dump.debug=true");
    else
        // restore categories --> previous filter restored
        QLoggingCategory::setFilterRules(cm_normal_filter);
}

