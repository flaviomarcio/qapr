#pragma once

#include <QObject>
#include <QThread>
#include <QVariant>
#include "../application/qapr_global.h"
#include "./qapr_notify_dispatch.h"
//#include "../../../qrpc/src/qrpc_setting_manager.h"
//#include "../../../qorm/src/qorm_connection_manager.h"
//#include "../application/qapr_consts.h"
//#include "./qapr_notify_base.h"

#define DECLARE_NOTIFY_DISPACHER(NotifyClass, methodName)\
static QApr::NotifyDispatch&methodName##_dispatch = QApr::Notify::instance().dispatcherRegister(NotifyClass::staticMetaObject, QByteArrayLiteral(#methodName));

namespace QApr{
class NotifyPvt;
//!
//! \brief The Notify class
//!
class Q_APR_EXPORT Notify : public QThread
{
    Q_OBJECT
public:



    //!
    //! \brief Notify
    //! \param parent
    //!
    Q_INVOKABLE explicit Notify(QObject*parent=nullptr);

    //!
    //! \brief resourceSettings
    //! \return
    //!
    Q_INVOKABLE static const QVariant resourceSettings();

    //!
    //! \brief instance
    //! \return
    //!
    static Notify &instance();


    void run() override;

    //!
    //! \brief start
    //! \return
    //!
    virtual bool start();

    //!
    //! \brief stop
    //! \return
    //!
    virtual bool stop();

    //!
    //! \brief serviceStart
    //! \param service
    //!
    virtual void serviceStart(const QByteArray &service);

    //!
    //! \brief dispatcherRegister
    //! \param metaObject
    //! \param name
    //! \return
    //!
    virtual NotifyDispatch &dispatcherRegister(const QMetaObject &metaObject, const QByteArray &name);

    //!
    //! \brief notify
    //! \param channel
    //! \param payload
    //! \return
    //!
    virtual bool notify(const QString &channel, const QVariant &payload);

private:
    NotifyPvt *p=nullptr;
};

//static auto &NotifyApp=Notify::instance();
}
