#ifndef M1_ENV_H
#define M1_ENV_H

#include <QTextStream>
#include <QFile>

namespace M1Store{

    class M1Env{
    private:
        // path to log file

        // static variables for the CSV output
        static QTextStream* cm_debug_stream;
        static QFile* cm_debug_file;

        // message handler function for qInstallMessageHandler()
        static void myMessageHandler(QtMsgType p_type, const QMessageLogContext &p_context, const QString & p_msg);
    public:
        // startup and closing function for the environment
        static void init();
        static void close();
    };

}
#endif // M1_ENV_H
