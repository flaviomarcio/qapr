#pragma once

#include <QObject>
#include <QVariant>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QCoreApplication>

#define Q_APR_DECLARE_INTERFACE_METHOD_CHECK()      \
public:                                             \
Q_INVOKABLE virtual QVariant check()                \
{                                                   \
    this->rq().co().setOK();                        \
    return {};                                      \
}                                                   \
Q_INVOKABLE virtual QVariant ping()                 \
{                                                   \
    this->rq().co().setOK();                        \
    return QDateTime::currentDateTime();            \
}                                                   \
Q_INVOKABLE virtual QVariant fullCheck()            \
{                                                   \
    this->rq().co().setOK();                        \
    return {};                                      \
}                                                   \
Q_INVOKABLE virtual QVariant connectionsCheck()     \
{                                                   \
    this->rq().co().setOK();                        \
    return {};                                      \
}                                                   \
Q_INVOKABLE virtual QVariant businessCheck()        \
{                                                   \
    this->rq().co().setNotImplemented();            \
    return {};                                      \
}