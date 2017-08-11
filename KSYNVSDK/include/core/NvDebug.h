//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Feb 14. 2014
//   Author:        NewAuto video team
//================================================================================

#pragma once

#include "HResult.h"
#include <QString>
#include <QByteArray>
#include <QtCore/qglobal.h>
#include <QtCore/qtextstream.h>
#include <QtCore/qstring.h>
#include <QObject>
#include <QFile>
#include <QString>
#include <QDate>
class CNvDebugLog;

class CNvMessageLogger {

    Q_DISABLE_COPY(CNvMessageLogger)
public:
    Q_DECL_CONSTEXPR CNvMessageLogger() : m_isLogger(false),m_line(0), m_file(Q_NULLPTR), m_function(Q_NULLPTR) {}
    Q_DECL_CONSTEXPR CNvMessageLogger(bool isLogger) : m_isLogger(isLogger),m_line(0), m_file(Q_NULLPTR), m_function(Q_NULLPTR) {}
    Q_DECL_CONSTEXPR CNvMessageLogger(bool isLogger,const char *fileName, int lineNumber, const char *functionName)
        :  m_isLogger(isLogger),m_line(lineNumber), m_file(fileName), m_function(functionName){}

    bool m_isLogger;
    int m_line;
    const char *m_file;
    const char *m_function;

    void debug(const char *msg, ...) const Q_ATTRIBUTE_FORMAT_PRINTF(2, 3);
    void error(const char *msg, ...) const Q_ATTRIBUTE_FORMAT_PRINTF(2, 3);
    void warning(const char *msg, ...) const Q_ATTRIBUTE_FORMAT_PRINTF(2, 3);

    CNvDebugLog debug() const;
    CNvDebugLog error() const;
    CNvDebugLog warning() const;
};

class CNvDebugLog
{

public:
    enum NvMessageType
    {
        NvMessageType_Debug,
        NvMessageType_Warning,
        NvMessageType_Error
    };

     CNvDebugLog(NvMessageType t,const char *file, int line, const char *function,bool isLogger);
     CNvDebugLog(const CNvDebugLog &o);
     CNvDebugLog &operator=(const CNvDebugLog &other);
    ~CNvDebugLog();

     // output
     static void debugFormatOutput(const char *msg, CNvDebugLog::NvMessageType type,bool isLogger);
     static void debugFormatOutput(const char *msg, CNvDebugLog::NvMessageType type,const char *file, int line, const char *function,bool isLogger);


     CNvDebugLog &operator<<(QChar t);
     CNvDebugLog &operator<<(bool t);
     CNvDebugLog &operator<<(char t);
     CNvDebugLog &operator<<(signed short t);
     CNvDebugLog &operator<<(unsigned short t);

     CNvDebugLog &operator<<(signed int t);
     CNvDebugLog &operator<<(unsigned int t);
     CNvDebugLog &operator<<(signed long t);
     CNvDebugLog &operator<<(unsigned long t);
     CNvDebugLog &operator<<(qint64 t);
     CNvDebugLog &operator<<(quint64 t);
     CNvDebugLog &operator<<(float t);
     CNvDebugLog &operator<<(double t);
     CNvDebugLog &operator<<(const char* t);
     CNvDebugLog &operator<<(const QString & t);
     CNvDebugLog &operator<<(const QStringRef & t);
     CNvDebugLog &operator<<(const QByteArray & t);
#ifdef Q_COMPILER_NULLPTR
     CNvDebugLog &operator<<(std::nullptr_t);
#endif

private:
     void DropStream();

private:
    struct Stream {
        Stream(NvMessageType t,const char *file, int line, const char *function,bool isLogger) : ts(&buffer, QIODevice::WriteOnly), ref(1), type(t),
             line(line),file(file),function(function),isLogger(isLogger),message_output(true) {}
        QTextStream ts;
        int ref;
        NvMessageType type;
        int line;
        const char *file;
        const char *function;
        bool isLogger;
        QString buffer;
        bool message_output;
    } *m_stream;

};

class CNvLogger : public QObject
{
    Q_OBJECT

private:
    CNvLogger(const QString &logFileBaseName);
    ~CNvLogger();

public:
    static void InitLogger(const QString &logFileBaseName = QString());
    static void CloseLogger();
    static void WriteMessage(const QString &message);
    static void WriteMessage(const QByteArray &message);
    static void WriteMessage(const char *message);

private:
    QString GenerateLogFilePath(const QDate &date);

private:
    virtual void timerEvent(QTimerEvent *e);
    virtual void customEvent(QEvent *e);

private:
    QString m_logFileBaseName;

    QFile *m_logFile;
    QDate m_logFileCreationDate;

    int m_timerId;
};

#ifdef QT_NO_DEBUG
#define NvDebugLog CNvMessageLogger(true).debug
#define NvErrorLog CNvMessageLogger(true).error
#define NvWarningLog CNvMessageLogger(true).warning
#else
#define NvDebugLog CNvMessageLogger(true,QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug
#define NvErrorLog CNvMessageLogger(true,QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).error
#define NvWarningLog CNvMessageLogger(true,QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).warning
#endif

#ifdef QT_NO_DEBUG
#define NvDebug CNvMessageLogger(false).debug
#define NvError CNvMessageLogger(false).error
#define NvWarning CNvMessageLogger(false).warning
#else
#define NvDebug CNvMessageLogger(false,QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug
#define NvError CNvMessageLogger(false,QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).error
#define NvWarning CNvMessageLogger(false,QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).warning
#endif

