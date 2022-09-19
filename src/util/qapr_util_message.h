#pragma once

#include "../../../qrpc/src/qrpc_service_setting.h"
#include "../application/qapr_global.h"

namespace QApr {

//!
//! \brief The UtilMessage class
//!
class Q_APR_EXPORT UtilMessage : public QStm::Object
{
public:
    //!
    //! \brief UtilMessage
    //! \param parent
    //!
    Q_INVOKABLE explicit UtilMessage(QObject *parent = nullptr);

    //!
    //! \brief send
    //! \param setting
    //! \param serviceType
    //! \param serviceToken
    //! \param to
    //! \param body
    //! \param attachment
    //! \return
    //!
    ResultValue &send(const QRpc::ServiceSetting &setting,
                      const QUuid &serviceToken,
                      const QByteArray &serviceType,
                      const QString &to,
                      const QString &body,
                      const QVariantList &attachment);

    //!
    //! \brief sendSMS
    //! \param setting
    //! \param serviceToken
    //! \param to
    //! \param body
    //! \return
    //!
    ResultValue &sendSMS(const QRpc::ServiceSetting &setting,
                         const QUuid &serviceToken,
                         const QString &to,
                         const QString &body);

    //!
    //! \brief sendPushNotify
    //! \param setting
    //! \param serviceToken
    //! \param to
    //! \param body
    //! \return
    //!
    ResultValue &sendPushNotify(const QRpc::ServiceSetting &setting,
                                const QUuid &serviceToken,
                                const QString &to,
                                const QString &body);

    //!
    //! \brief sendTelegram
    //! \param setting
    //! \param serviceToken
    //! \param to
    //! \param body
    //! \return
    //!
    ResultValue &sendTelegram(const QRpc::ServiceSetting &setting,
                              const QUuid &serviceToken,
                              const QString &to,
                              const QString &body);

    //!
    //! \brief sendEmail
    //! \param setting
    //! \param serviceToken
    //! \param to
    //! \param body
    //! \return
    //!
    ResultValue &sendEmail(const QRpc::ServiceSetting &setting,
                           const QUuid &serviceToken,
                           const QString &to,
                           const QString &body);

    //!
    //! \brief sendWhatsApp
    //! \param setting
    //! \param serviceToken
    //! \param to
    //! \param body
    //! \return
    //!
    ResultValue &sendWhatsApp(const QRpc::ServiceSetting &setting,
                              const QUuid &serviceToken,
                              const QString &to,
                              const QString &body);

    //!
    //! \brief send
    //! \param setting
    //! \param message
    //! \return
    //!
    ResultValue &send(const QRpc::ServiceSetting &setting, const QVariant &message);

    //!
    //! \brief sendSMS
    //! \param setting
    //! \param message
    //! \return
    //!
    ResultValue &sendSMS(const QRpc::ServiceSetting &setting, const QVariant &message);

    //!
    //! \brief sendPushNotify
    //! \param setting
    //! \param message
    //! \return
    //!
    ResultValue &sendPushNotify(const QRpc::ServiceSetting &setting, const QVariant &message);

    //!
    //! \brief sendTelegram
    //! \param setting
    //! \param message
    //! \return
    //!
    ResultValue &sendTelegram(const QRpc::ServiceSetting &setting, const QVariant &message);

    //!
    //! \brief sendEmail
    //! \param setting
    //! \param message
    //! \return
    //!
    ResultValue &sendEmail(const QRpc::ServiceSetting &setting, const QVariant &message);

    //!
    //! \brief sendWhatsApp
    //! \param setting
    //! \param message
    //! \return
    //!
    ResultValue &sendWhatsApp(const QRpc::ServiceSetting &setting, const QVariant &message);
};

} // namespace QApr
