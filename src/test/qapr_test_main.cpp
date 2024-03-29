#include "qapr_test_main.h"
#include "../../../qrpc/src/qrpc_listen_protocol.h"
#include "../../../qrpc/src/qrpc_server.h"
#include "../application/qapr.h"

namespace QApr {

TestMain::TestMain(QObject *parent)
    : QObject{parent}
{

}

void TestMain::configure()
{
    QVERIFY(this->serviceStart());
}

void TestMain::deconfigure()
{
    QVERIFY(this->serviceStop());
}

void TestMain::execute()
{

}

void TestMain::run()
{
    QApr::Application::lockedWait();
    this->configure();

    this->execute();

    this->deconfigure();
}

bool TestMain::serviceStart()
{
#ifdef QTREFORCE_QAPR
    auto &server=QApr::Server::i();
    auto listaner=server.colletions().protocol(QRpc::Types::Http);
    if(!listaner)
        return false;
    listaner->setPort(9999);
    auto __return=server.start();
    QThread::msleep(2000);
    return __return;
#else
    return false;
#endif
}

bool TestMain::serviceStop()
{
#ifdef QTREFORCE_QAPR
    auto &server=QApr::Server::i();
    return server.stop();
#else
    return false;
#endif
}

} // namespace QApr
