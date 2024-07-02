#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <iostream>

#include "m1A_env.h"
#include "m1A_constants.h"

bool M1Env::M1EnvStatic::cm_block = false;

/**
 * @brief M1Store::M1Env::myMessageHandler message handler callback for Qt debug infra
 * @param p_type log level
 * @param p_context function name, file, line, etc
 * @param p_msg the message
 */
void M1Env::M1EnvStatic::myMessageHandler(QtMsgType p_type, const QMessageLogContext &p_context, const QString & p_msg)
{
    if( cm_block ) return;

    // Level message decoding
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
                            .arg(l_level, p_context.category, QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz"))
                            .arg(p_context.function, indentSpace() + l_msg)
                            //.arg(p_context.function, indentSpace() + l_msg + QString(" [cm_depth = %1]").arg(cm_depth))
                            .arg(p_context.file).arg(p_context.line);

    // CSV message to stream
    *cm_debug_stream << l_txt_csv << Qt::endl;

    if(! cm_list_excluded_catergory_for_screen.contains(p_context.category)){
        // message for screen debug
        QString l_txt_screen = QString("[%1] [%2] %3 (%4 %5:%6) %7")
                                   .arg(l_level, p_context.category, QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                                   .arg(p_context.file, p_context.function)
                                   .arg(p_context.line).arg(l_msg);

        // screen message to screen
        std::cout << l_txt_screen.toStdString() << std::endl << std::flush;
    }
}

int M1Env::M1EnvStatic::cm_depth = 0;

Q_LOGGING_CATEGORY(g_dummy_cat, "dummy")
auto g_cat = g_dummy_cat;

QStringList M1Env::M1EnvStatic::cm_list_excluded_catergory_for_screen;

void M1Env::M1EnvStatic::addExcludeCatergoryForScreen(QString p_cat){
    cm_list_excluded_catergory_for_screen.append(p_cat);
}


QString M1Env::M1EnvStatic::indentSpace(){
    // return QString("[%1] ").arg(cm_depth) + QString("...").repeated(cm_depth);
    return QString("...").repeated(cm_depth);
}

void M1Env::M1EnvStatic::entry(const QLoggingCategory& p_cat(),
                               const QString& p_file,
                               int p_line,
                               const QString& p_func){

    g_cat = p_cat;
    cm_depth += 1;
    // qCDebug(g_cat) << QString("cm_depth is now %1").arg(cm_depth);
}
void M1Env::M1EnvStatic::exit(
    const QString& p_file,
    int p_line,
    const QString& p_func){

    // qCDebug(g_cat) << QString("cm_depth was %1").arg(cm_depth);
    cm_depth -= 1;
    qCDebug(g_cat) << QString("[Exiting %1()]").arg(p_func);
}

/**
 * @brief M1Store::M1Env::init init m1 env
 */
void M1Env::M1EnvStatic::init(){
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
void M1Env::M1EnvStatic::close(){
    // last debug message
    qDebug() << "Closing Logging system";

    // flush and close debug stream
    cm_debug_stream->flush();
    delete cm_debug_stream;
    delete cm_debug_file;
    cm_block = true;
}

// default inits for static members
QTextStream* M1Env::M1EnvStatic::cm_debug_stream = 0;
QFile* M1Env::M1EnvStatic::cm_debug_file = 0;
