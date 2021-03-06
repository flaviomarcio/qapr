#include "./qapr_notify.h"
#include "./qapr_notify_base.h"
#include "../../../qorm/src/qorm_connection_notify.h"
#include "../application/qapr_application.h"
#include <QCryptographicHash>
#include <QMultiHash>
#include <QMutex>
#include <QTimer>

namespace QApr {

Q_GLOBAL_STATIC(Notify, staticNotify)

class NotifyPvt: public QObject{
public:
    QMutex mutexNotify;
    Notify *notify=nullptr;
    QHash<QByteArray, const QMetaObject*> services;
    QHash<QByteArray, NotifyDispatch*> dispatchers;
    QHash<QByteArray, NotifyBase*> tasks;

    QOrm::ConnectionNotify connectionNotify;

    explicit NotifyPvt(Notify*parent) : QObject{parent}, notify(parent)
    {
        QObject::connect(&connectionNotify, &QOrm::ConnectionNotify::notification, this, &NotifyPvt::taskReceived);

    }
    void taskRun(const QByteArray &service)
    {
        auto task = this->tasks.value(service);
        if(task==nullptr){
            auto metaObject=this->services.value(service);
            auto object=metaObject->newInstance(Q_ARG(QObject*, nullptr ));
            if(object!=nullptr){
                task=dynamic_cast<NotifyBase*>(object);
                if(task==nullptr)
                    delete object;
                else{
                    this->tasks.insert(service, task);
                    task->setNotifyName(service);
                }
            }
        }

        if(task!=nullptr){
            if(!task->notifySetting().enabled()){
                if(task!=nullptr){
                    task->quit();
                    task->wait(1000);
                }
#if Q_APR_LOG_SUPER_VERBOSE
                qInfo()<<"Job is disabled: service name=="<<service<<", NotifySetting.name()=="<<NotifySetting.name();
#endif
            }
            else{
#if Q_APR_LOG_SUPER_VERBOSE
                qInfo()<<"Job is enabled: "<<NotifySetting.name();
#endif
            }
        }

    }

    bool taskStart()
    {
        if(this->connectionNotify.start()){
            QMutexLocker<QMutex> locker(&mutexNotify);
            QHashIterator<QByteArray, const QMetaObject*> i(this->services);
            while (i.hasNext()) {
                i.next();
                auto key=i.key();
                this->taskRun(key);
            }
            return true;
        }
        return false;
    }

    void taskFinish()
    {
        for(auto &v:tasks){
            v->quit();
            if(v->wait(1000))
                delete v;
            else
                v->deleteLater();
        }
    }

    NotifyDispatch&taskRegister(const QMetaObject&metaObject, const QByteArray &methodName)
    {
        auto mthName=methodName.trimmed();
        if(!mthName.isEmpty()){
            auto mtdMd5=QCryptographicHash::hash(mthName, QCryptographicHash::Md5).toHex();
            this->services.insert(mthName, &metaObject);
            auto dispatch=this->dispatchers.value(mthName);
            dispatch=dispatch?dispatch:new NotifyDispatch(mthName, mtdMd5, this);
            this->dispatchers.insert(dispatch->channel(), dispatch);
            return*dispatch;
        }
        static NotifyDispatch __NotifyDispatch;
        __NotifyDispatch.clear();
        return __NotifyDispatch;
    }

    bool taskNotify(const QString &channel, const QVariant &payload)
    {
        return connectionNotify.notify_send(channel, payload);
    }

    void taskReceived(const QString &channel, const QVariant &payload)
    {
        auto name=channel.toUtf8();
        auto service=this->tasks.value(name);
        if(service!=nullptr){
            emit service->notifyReceived(channel, payload);
        }
    }
};

Notify::Notify(QObject*parent):QThread{nullptr}
{
    Q_UNUSED(parent)
    this->p = new NotifyPvt{this};
}

const QVariant Notify::resourceSettings()
{
    return QApr::Application::i().resourceSettings();
}

Notify &Notify::instance()
{
    return *staticNotify;
}

void Notify::run()
{
    if(p->taskStart())
        this->exec();
    p->taskFinish();
}

bool Notify::start()
{
#ifdef QAPR_LOG_VERBOSE
    oWarning()<<tr("started");
#endif
    auto objectName=this->objectName().trimmed();
    if(objectName.isEmpty()){
        objectName=this->metaObject()->className();
        while(objectName.contains(QStringLiteral(":")))
            objectName=objectName.replace(QStringLiteral(":"), QStringLiteral("_"));
        while(objectName.contains(QStringLiteral("__")))
            objectName=objectName.replace(QStringLiteral("__"), QStringLiteral("_"));
    }
    this->setObjectName(objectName);
    QThread::start();
    while(this->eventDispatcher()==nullptr)
        QThread::msleep(1);
    return true;
}

bool Notify::stop()
{
    this->quit();
    return true;
}

void Notify::serviceStart(const QByteArray &service)
{
    p->taskRun(service);
}

NotifyDispatch&Notify::dispatcherRegister(const QMetaObject &metaObject, const QByteArray &name)
{
    return p->taskRegister(metaObject, name);
}

bool Notify::notify(const QString &channel, const QVariant &payload)
{
    return !this->isRunning()?false:p->taskNotify(channel, payload);
}


}
