#ifndef M1_ENV_H
#define M1_ENV_H

#include <QTextStream>
#include <QFile>
#include <QLoggingCategory>
#include <stack>

#define M1_FUNC_ENTRY(a, b) M1Env::M1EnvStatic::entry(a, b, __FILE__, __LINE__, __PRETTY_FUNCTION__, __FUNCTION__);
#define M1_FUNC_EXIT M1Env::M1EnvStatic::exit(__FILE__, __LINE__, __PRETTY_FUNCTION__, __FUNCTION__);

namespace M1Env{

class M1EnvStatic{
    private:
        // static variables for the CSV output
        static QTextStream* cm_debug_stream;
        static QFile* cm_debug_file;
        static bool cm_block;
        static int cm_depth;
        static QStringList cm_list_excluded_catergory_for_screen;
        static std::stack<QString> cm_current_category;
        static QString cm_normal_filter;

        // message handler function for qInstallMessageHandler()
        static void myMessageHandler(QtMsgType p_type, const QMessageLogContext &p_context, const QString & p_msg);
        static void sendToLogChannels(QtMsgType p_type,
                                      const QString & p_msg,
                                      const QString & p_cat,
                                      const QString& p_file,
                                      int p_line,
                                      const QString& p_func);
    public:
        // startup and closing function for the environment
        static void init();
        static void close();
        static void entry(const QLoggingCategory& p_log_cat(),
                          const QString& p_msg,
                          const QString& p_file,
                          int p_line,
                          const QString& p_func_deco,
                          const QString& p_func);
        static void exit(const QString& p_file,
                         int p_line,
                         const QString& p_func_deco,
                         const QString& p_func);

        static QString indentSpace();
        static void addExcludeCatergoryForScreen(QString p_cat);

        static void setSilentMode(bool p_set);
        static void setNormalFilter(const QString& p_filter);
    };

    template <typename T>
    QString vectorToString(QList<T>& p_list){
        QStringList l_list;
        for(T l_element : p_list)
            l_list.append(l_element->dbgShort());

        return QString("[") + l_list.join(", ") + "]";
    }
}
#endif // M1_ENV_H
