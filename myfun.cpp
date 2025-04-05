#include "myfun.hpp"

char* QString2char(QString s)
{
    char *chStr = NULL;
    QByteArray ba = s.toUtf8();
    chStr = (char *)malloc(ba.length() + 1);
    memset(chStr, 0, ba.length());
    memcpy(chStr, ba.data(), ba.length());
    chStr[ba.length()] = '\0';
    return chStr;
}

QString MyExec(QString cmd, QString execDir=QDir::homePath())
{
    QProcess p;
    QStringList arg;
    char* chStr;

    chStr = QString2char(cmd);

    #if defined (Q_OS_LINUX)
        p.setProgram("bash");
        arg << "-c" << chStr ;
    #elif defined (Q_OS_WIN32)
        p.setProgram("cmd");
        arg << "/c" << chStr ;
    #endif

        p.setWorkingDirectory(execDir);
        p.setArguments(arg);
        p.start();
        p.waitForStarted();
        p.waitForFinished();

        QString errMsg = QString::fromUtf8(p.readAllStandardError());
        if (errMsg == "") {
            // 有些错误信息，只打印在标准输出。
            errMsg = QString::fromUtf8(p.readAllStandardOutput());
            if(errMsg.contains("FirstPage > LastPage")) {
                ;
            }
            else if(errMsg.contains("Error"))
            {
                ;
            }
            else
            {
                errMsg = "";
            }
        }

        free(chStr);
        p.close();

        return errMsg;
}
