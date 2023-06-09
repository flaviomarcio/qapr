#include "./qapr_scheduler_agent.h"
#include "../application/qapr_macro.h"
#include "../application/qapr_application.h"
#include "../../../qstm/src/qstm_util_variant.h"
#include "./qapr_scheduler.h"
#include "./qapr_scheduler_task.h"
#include <QMultiHash>
#include <QMutex>
#include <QTimer>

namespace QApr {


static const auto __services="services";
static const auto __agent="agent";
static const auto __default="default";

static auto __make_methodBlackList()
{
    QList<QByteArray> __return=QAPR_METHOD_BACK_LIST;
    auto metaObject=QStm::Object::staticMetaObject;
    for (int index = 0; index < metaObject.superClass()->methodCount(); ++index){
        auto method=metaObject.method(index);
        __return.append(method.name());
    }
    return __return;
}

Q_GLOBAL_STATIC_WITH_ARGS(QList<QByteArray>, methodBlackList, (__make_methodBlackList()));
Q_GLOBAL_STATIC(SchedulerAgent, staticAgent);

class SchedulerAgentPvt: public QObject{
public:
    SchedulerAgent *agent=nullptr;
    QHash<int, const QMetaObject*> services;
    QHash<QByteArray, Schedule*> schedulers;
    QHash<QByteArray, QDateTime> tasksInterval;
    QHash<QUuid, SchedulerTask*> tasks;

    explicit SchedulerAgentPvt(SchedulerAgent *parent) : QObject{parent}, agent{parent}
    {
        this->agent=parent;
    }

public slots:

    void start()
    {
        this->free();

        QHashIterator <int, const QMetaObject*> i(this->services);
        while(i.hasNext()){
            i.next();
            const auto metaObject=i.value();
            auto method=metaObject->method(i.key());
            scheduleCreate(method, *metaObject);
        }

        {//start
            QHashIterator <QUuid, SchedulerTask*> i(this->tasks);
            while(i.hasNext()){
                i.next();
                if(!i.value()->isRunning())
                    i.value()->start();
            }
        }
    }

    void free()
    {
        auto lst=tasks.values();
        tasks.clear();
        for(auto &task: lst){
            task->quit();
            if(task->wait(1000))
                delete task;
            else
                task->deleteLater();
        }
    }

    bool scheduleCreate(const QMetaMethod &method, const QMetaObject &metaObject)
    {
        QScopedPointer<QObject> sObj(metaObject.newInstance(Q_ARG(QObject*, this )));
        if(!sObj.data())
            return false;
        auto scheduler=dynamic_cast<Scheduler*>(sObj.data());
        if(!scheduler)
            return false;

        auto vSettings=QApr::Application::i().manager().settingBody(__services);
        {
            Q_DECLARE_VU;
            auto vSettingsDefault=vSettings.value(__default).toHash();
            //auto vSettingsAgent=vSettings.value(__agent).toHash();
            auto vSettingsService=vSettings.value(method.name()).toHash();
            vSettings=vu.vMerge(vSettingsDefault, vSettingsService).toHash();
        }

        const auto &annotations = scheduler->annotation(method);

        if(!annotations.contains(agent->scSchedule()))
            return false;

        auto scEnabled = annotations.find(agent->scEnabled()).value();
        auto scIntervalLimit = annotations.find(agent->scIntervalLimit()).value();
        auto scIntervalInitial = annotations.find(agent->scIntervalInitial()).value();
        auto scInterval = annotations.find(agent->scInterval()).value();

        QStm::Envs envs;
        envs.customEnvs(vSettings);
        scEnabled = envs.parser(scEnabled);
        scIntervalLimit = envs.parser(scIntervalLimit);
        scIntervalInitial = envs.parser(scIntervalInitial);
        scInterval = envs.parser(scInterval);

        if (scEnabled.isValid() && !scEnabled.toBool())
            return false;

        static const auto t10m="10m";
        static const auto t1m="1m";
        static const auto t100ms="100ms";

        scInterval=(scInterval.isValid())?scInterval:t1m;
        scIntervalInitial=(scIntervalInitial.isValid())?scIntervalInitial:t100ms;
        scIntervalLimit=(scIntervalLimit.isValid())?scIntervalLimit:t10m;

        auto task=SchedulerTask::builder(this)
                        .name(method.name())
                        .taskMetaMethod(method)
                        .taskMetaObject(metaObject)
                        .build();

        if(!this->tasks.contains(task->uuid())){

        }

        task->settings().clear();
        task->settings().setActivityInterval(scInterval);
        task->settings().setActivityIntervalInitial(scIntervalInitial);
        task->settings().setActivityLimit(scIntervalLimit);
        task->settings().setEnabled(scEnabled.toBool());

        this->tasks.insert(task->uuid(), task);
        return true;
    }


    bool serviceRegister(const QMetaObject *metaObject)
    {
        if (!metaObject->inherits(&Scheduler::staticMetaObject))
            return false;

        QScopedPointer<QObject> sObj(metaObject->newInstance(Q_ARG(QObject*, this )));
        if(!sObj.data())
            return false;
        auto scheduler=dynamic_cast<Scheduler*>(sObj.data());
        if(!scheduler)
            return false;

        const auto &annotations = scheduler->annotation();

        if(!annotations.contains(agent->scObject()))
            return false;

        auto vSettings=QApr::Application::i().manager().settingBody();
        vSettings=vSettings.value(__services).toHash();

        for (int index = 0; index < metaObject->methodCount(); ++index) {
            auto method=metaObject->method(index);

            if(method.methodType()!=QMetaMethod::Method)
                continue;

            auto methodName=method.name();

            if(methodBlackList->contains(methodName))
                continue;

            const auto &annotations = scheduler->annotation(method);

            if(!annotations.contains(agent->scSchedule()))
                continue;

            this->services.insert(index, metaObject);

        }
        return !services.isEmpty();
    }

};

SchedulerAgent::SchedulerAgent(QObject *parent):QThread{nullptr}, PrivateQApr::SchedulerAnnotation{this}
{
    Q_UNUSED(parent)
    this->p = new SchedulerAgentPvt{this};
    this->moveToThread(this);
}

SchedulerAgent &SchedulerAgent::i()
{
    return *staticAgent;
}

void SchedulerAgent::run()
{
    aDebug()<<QStringLiteral("Scheduler running");
    QTimer::singleShot(100,this,[this](){
        p->start();
    });
    this->exec();
    p->free();
    aDebug()<<QStringLiteral("Scheduler finished");
 }

bool SchedulerAgent::start()
{
#ifdef QAPR_LOG_VERBOSE
    aWarning()<<QStringLiteral("started");
#endif
    QThread::start();
    while(this->eventDispatcher()==nullptr)
        QThread::msleep(1);
    return true;
}

bool SchedulerAgent::stop()
{
    this->quit();
    return true;
}

bool SchedulerAgent::serviceRegister(const QMetaObject &metaObject)
{
    return p->serviceRegister(&metaObject);
}

}
