#pragma once

#if QTREFORCE_QAPIDOC
#include "../../../qapidoc/includes/QApiDoc"
#endif
#include "../../../qrpc/src/qrpc_controller.h"
#include "../application/qapr_global.h"
#include "../application/qapr_macro.h"
#include "./qapr_interface_annotations.h"

namespace QApr {

//!
//! \brief The InterfaceMaintence class
//!
class Q_APR_EXPORT InterfaceMaintence : public QRpc::Controller,
                                        public QAprPrivate::InterfaceAnnotations
{
    Q_OBJECT
    QAPR_DECLARE_INTERFACE_METHOD_CHECK()
    Q_ANNOTATION(InterfaceMaintence, QVariantList({apiBasePath("/maintence")}))

#if QTREFORCE_QAPIDOC
    Q_API_DOC_INFO()
    {
        document->host(QStringLiteral("localhost"))
            .basePath(this->basePath())
            .consumes(QStringLiteral("application/json"))
            .produces(QStringLiteral("application/json"))
            .schemes(stpsHttp);

        document->info()
            .title(QStringLiteral("QApr API for custom maintenance"))
            .version(QStringLiteral("1.0.0"))
            .termsOfService(QStringLiteral("http://www.apache.org/licenses/LICENSE-2.0.txt"))
            .description(QStringLiteral("Inherited from QRpc::Controller, this is the class for builder "
                             "interface for maintenance"));

        document->info()
            .contact()
            .name(QStringLiteral("Flavio Portela"))
            .email(QStringLiteral("fmspx@hotmail.com"))
            .url(QStringLiteral("https://github.com/flaviomarcio/qtreforce-sdk"));

        document->info()
            .license()
            .name(QStringLiteral("Apache License - Version 2.0, January 2004"))
            .url(QStringLiteral("http://www.apache.org/licenses/LICENSE-2.0"));
    }
#endif
public:
    //!
    //! \brief InterfaceMaintence
    //! \param parent
    //!
    Q_INVOKABLE explicit InterfaceMaintence(QObject *parent = nullptr);

    //!
    //! \brief serverStart
    //! \return
    //!
    Q_ANNOTATION(serverStart, QVariantList({opPost, opPut}))
    Q_INVOKABLE virtual QVariant serverStart();
#if QTREFORCE_QAPIDOC
    Q_API_DOC_PATH(serverStart)
    {
        path->operation(sptoGet)
            .operationId({})
            .description(QStringLiteral("Start custom services"))
            .responses(QApiResponse().statusCode(200).examples(
                QVariantHash{{QStringLiteral("response"), QStringLiteral("custom implementation")}}));
    }
#endif
    //!
    //! \brief serverStop
    //! \return
    //!
    Q_ANNOTATION(serverStop, QVariantList({opPost, opPut}))
    Q_INVOKABLE virtual QVariant serverStop();
#if QTREFORCE_QAPIDOC
    Q_API_DOC_PATH(serverStop)
    {
        path->operation(sptoGet)
            .operationId({})
            .description(QStringLiteral("Stop custom services"))
            .responses(QApiResponse().statusCode(200).examples(
                QVariantHash{{QStringLiteral("response"), QStringLiteral("custom implementation")}}));
    }
#endif
    //!
    //! \brief serverRestart
    //! \return
    //!
    Q_ANNOTATION(serverRestart, QVariantList({opPost, opPut}))
    Q_INVOKABLE virtual QVariant serverRestart();
#if QTREFORCE_QAPIDOC
    Q_API_DOC_PATH(serverRestart)
    {
        path->operation(sptoGet)
            .operationId({})
            .description(QStringLiteral("Restart custom services"))
            .responses(QApiResponse().statusCode(200).examples(
                QVariantHash{{QStringLiteral("response"), QStringLiteral("custom implementation")}}));
    }
#endif
    //!
    //! \brief servicesCheck
    //! \return
    //!
    Q_ANNOTATION(servicesCheck, QVariantList({opPost, opPut}))
    Q_INVOKABLE virtual QVariant servicesCheck();
#if QTREFORCE_QAPIDOC
    Q_API_DOC_PATH(servicesCheck)
    {
        path->operation(sptoGet)
            .operationId({})
            .description(QStringLiteral("Check custom services"))
            .responses(QApiResponse().statusCode(200).examples(
                QVariantHash{{QStringLiteral("response"), QStringLiteral("custom implementation")}}));
    }
#endif
    //!
    //! \brief applicationQuit
    //! \return
    //!
    Q_ANNOTATION(applicationQuit, QVariantList({opPost, opPut}))
    Q_INVOKABLE virtual QVariant applicationQuit();
#if QTREFORCE_QAPIDOC
    Q_API_DOC_PATH(applicationQuit)
    {
        path->operation(sptoGet)
            .operationId({})
            .description(QStringLiteral("Quit application"))
            .responses(QApiResponse().statusCode(200).examples(
                QVariantHash{{QStringLiteral("response"), QStringLiteral("custom implementation")}}));
    }
#endif
signals:
};

//QRPC_CONTROLLER_AUTO_REGISTER(InterfaceMaintence)

} // namespace QApr
