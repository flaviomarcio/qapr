#include "./qapr_mainservice.h"
#include "./qapr_application.h"
#include "../services/qapr_server.h"
#include "../services/qapr_agent.h"
#include "../services/qapr_notify.h"
#include <QProcess>
#include <QDir>

namespace QApr {

MainService::MainService(QObject *parent):QObject{parent}
{
    QLocale::setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
}

int MainService::exec(QCoreApplication &a)
{
    Q_UNUSED(a)
    bool __return=false;

    auto &appInstance=Application::i();

    const auto &arguments = appInstance.arguments();

    if(arguments.contains(QStringLiteral("ws"))){
        auto &service = Server::instance();
        service.start();
        __return = service.isRunning() || __return;
    }

    if(arguments.contains(QStringLiteral("agent"))){
        auto &service = Agent::instance();
        service.start();
        __return = service.isRunning() || __return;
    }

    if(arguments.contains(QStringLiteral("notify"))){
        auto &service = Notify::instance();
        service.start();
        __return = service.isRunning() || __return;
    }

    if(__return)
        return appInstance.exec(a);

    return QProcess::NormalExit;
}

}
