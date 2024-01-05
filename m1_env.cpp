#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <iostream>

#include "m1_env.h"

/**
 * @brief M1Store::M1Env::myMessageHandler message handler callback for Qt debug infra
 * @param p_type log level
 * @param p_context function name, file, line, etc
 * @param p_msg the message
 */
void M1Store::M1Env::myMessageHandler(QtMsgType p_type, const QMessageLogContext &p_context, const QString & p_msg)
{
    // Level message decoding
    QString l_level;
    if(p_type == QtDebugMsg) l_level = "Debug";
    else if(p_type == QtWarningMsg) l_level = "Warning";
    else if(p_type == QtCriticalMsg) l_level = "Critical";
    else if(p_type == QtFatalMsg) l_level = "Fatal";

    // remove double quotes from message if any (more double quotes will be added below)
    QString l_msg = (p_msg.at(0) == '"' && p_msg.at(p_msg.length()-1) == '"') ? p_msg.sliced(1, p_msg.length()-2) : p_msg;

    // message for screen debug
    QString l_txt_screen = QString("[%1] %2 (%3 %4:%5) %6")
                               .arg(l_level, QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
                               .arg(p_context.file, p_context.function)
                               .arg(p_context.line).arg(l_msg);

    // CSV message
    l_msg.replace("\"", "\"\""); // double quote escape
    QString l_txt_csv = QString("\"%1\";\"%2\";\"%3\";\"%4\";\"%5\";\"%6\";\"%7\"")
                            .arg(l_level, p_context.category, QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss.zzz"))
                            .arg(p_context.file).arg(p_context.line)
                            .arg(p_context.function, l_msg);

    // CSV message to stream
    *cm_debug_stream << l_txt_csv << Qt::endl;
    // screen message to screen
    std::cout << l_txt_screen.toStdString() << std::endl << std::flush;
}

/**
 * @brief M1Store::M1Env::init init m1 env
 */
void M1Store::M1Env::init(){
    // open stream for CSV debug messages
    cm_debug_file = new QFile(M1Store::LOG_FILE_NAME);
    cm_debug_file->open(QIODevice::WriteOnly);
    cm_debug_stream = new QTextStream(cm_debug_file);

    // declare the message handler callback to Qt Log
    qInstallMessageHandler(myMessageHandler); // Install the handler

    // initial debug message
    qDebug() << "Logger set-up complete";
}
/**
 * @brief M1Store::M1Env::close close m1 env
 */
void M1Store::M1Env::close(){
    // last debug message
    qDebug() << "Closing Logging system";

    // flush and close debug stream
    cm_debug_stream->flush();
    delete cm_debug_stream;
    delete cm_debug_file;
}

// default inits for static members
QTextStream* M1Store::M1Env::cm_debug_stream = 0;
QFile* M1Store::M1Env::cm_debug_file = 0;
