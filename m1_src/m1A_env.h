#ifndef M1_ENV_H
#define M1_ENV_H

#include <QTextStream>
#include <QFile>
#include <QLoggingCategory>

#include <stack>
// #include <exception>

extern QRegularExpression g_re_space;
extern QRegularExpression g_re_punc;
extern QRegularExpression g_re_tags;
extern QRegularExpression g_re_initial_p;
extern QRegularExpression g_re_final_p;
extern QRegularExpression g_re_initial_table;
extern QRegularExpression g_re_final_table;
extern QRegularExpression g_re_capture_gammar;
extern QRegularExpression g_re_cap_initial;
extern QRegularExpression g_re_sent_num;
extern QRegularExpression g_re_stephanus;
extern QRegularExpression g_re_no_base;
extern QRegularExpression g_re_extract_vt;
extern QRegularExpression g_re_html_tags;

/**
 * \defgroup EntryExit function Entry and Exit macros
 * @ingroup DebugLog
 * @{
 */
#ifndef QT_NO_DEBUG_OUTPUT
#define M1_FUNC_ENTRY(a, b) M1Env::EnvStatic::entry(a, b, __FILE__, __LINE__, __PRETTY_FUNCTION__, __FUNCTION__);
#define M1_FUNC_EXIT M1Env::EnvStatic::exit(__FILE__, __LINE__, __PRETTY_FUNCTION__, __FUNCTION__);
#else
#define M1_FUNC_ENTRY(a, b)
#define M1_FUNC_EXIT
#endif
/**@}*/ //end of EntryExit

namespace M1Env{

/**
 * @brief The M1Exception class
 */

class M1Exception : public std::exception{
private:
    QString m_message;
    QByteArray m_tmp; // for compatibility with std::exception::what()
    unsigned long m_code;
public:
    M1Exception(QString p_message, unsigned long p_code){m_message = p_message; m_code = p_code; m_tmp = qWhat().toUtf8();}

    QString qWhat() const {return QString("[%1] %2").arg(m_code).arg(m_message);}
    QString message() const {return m_message;}
    unsigned long code() const {return m_code;}

    virtual char* what(){return m_tmp.data();}
};


/**
 * @brief The M1EnvStatic class
 * @ingroup DebugLog
 */
class EnvStatic{
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
        static void setTmpDebug(bool p_on);
};

template <typename T>
QString vectorToString(QList<T>& p_list){
    QStringList l_list;
    for(T l_element : p_list)
        l_list.append(l_element->dbgShort());

    return QString("[") + l_list.join(", ") + "]";
}

}// end namespace M1Env
#endif // M1_ENV_H
