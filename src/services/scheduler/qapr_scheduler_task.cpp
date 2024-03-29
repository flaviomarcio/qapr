#include "./qapr_scheduler_task.h"
#include "../application/qapr_consts.h"
#include "../application/qapr_macro.h"
#include "../../qstm/src/qstm_setting_base.h"
#include <QUuid>
#include <QTimer>
#include <QMutex>

namespace QApr {

Q_GLOBAL_STATIC(QMutex, staticMutexSynchronize)

class SchedulerTaskPvt: public QObject{
public:

    SchedulerTask *parent=nullptr;
    SchedulerScopeGroup *scope=nullptr;
    QUuid uuid={};
    QTimer *timer=nullptr;
    QStm::SettingBase settings;
    QVariantHash stats;
    QDateTime lastExec;
    bool synchronize=false;
    explicit SchedulerTaskPvt(SchedulerTask *parent, SchedulerScopeGroup *scope)
        :
        QObject{parent},
        parent{parent},
        scope{scope},
        synchronize(scope?scope->synchronize():false)
    {
        if(scope){
            this->uuid=scope->uuid();
            this->setObjectName(QString("SchTsk_%1_%2").arg(scope->scope(), scope->group()));
        }
    }

    void timerStop()
    {
        if(this->timer==nullptr)
            return;
        QObject::disconnect(timer, &QTimer::timeout, this, &SchedulerTaskPvt::taskRun);
        this->timer->stop();
        this->timer->deleteLater();
        this->timer=nullptr;
    }


    void timerStart()
    {
        timerStop();
        auto interval=this->settings.activityInterval();
        if(interval<=0)
            return;

        timer=new QTimer();
        timer->setInterval(interval);
        QObject::connect(timer, &QTimer::timeout, this, &SchedulerTaskPvt::taskRun);
        timer->start();

    }

public slots:

    void taskRun()
    {
        if(this->synchronize)
            staticMutexSynchronize->lock();
        this->timerStop();
#ifdef Q_APR_LOG
        aInfo()<<QStringLiteral("Scheduler[%1]: started").arg(scope->scope());
#endif
        if(scope==nullptr){
            aWarning()<<QStringLiteral("Scheduler[%1]: invalid scope").arg(this->scope->scope());
        }
        else{
            this->lastExec=QDateTime::currentDateTime();
            scope->invoke();
        }
        auto totalTime=QDateTime::currentDateTime().toMSecsSinceEpoch()-this->lastExec.toMSecsSinceEpoch();
        this->timerStart();
#ifdef Q_APR_LOG
        aInfo()<<QStringLiteral("Scheduler[%1]: finished, total-time[%2 ms]").arg(this->scope->scope(), QString::number(totalTime));
#endif
        if(this->synchronize)
            staticMutexSynchronize->unlock();
    }

};

SchedulerTask::SchedulerTask(QObject *parent):QThread{nullptr},p{new SchedulerTaskPvt{this, nullptr}}
{
    Q_UNUSED(parent)
    this->moveToThread(this);
}

SchedulerTask::SchedulerTask(SchedulerScopeGroup *scope):QThread{nullptr},p{new SchedulerTaskPvt{this, scope}}
{
    this->moveToThread(this);
}

void SchedulerTask::run()
{
#ifdef Q_APR_LOG_VERBOSE
    aWarning()<<QStringLiteral("started");
#endif
    auto interval=p->settings.activityInterval();
    if(interval<=0){
        aDebug()<<QStringLiteral("Scope:[%1-%2]: interval: %3, state: stoped").arg(p->scope->scope(), p->scope->group()).arg(interval);
    }
    else{
        auto intervalInitial=p->settings.activityIntervalInitial();
        intervalInitial=(intervalInitial>0)?intervalInitial:100;
        QTimer::singleShot(intervalInitial, this, [this](){
            p->taskRun();
        });
        aDebug()<<QStringLiteral("Scope:[%1,%2]: initialStart: %3, state: running ").arg(p->scope->scope(), p->scope->group()).arg(intervalInitial);
        this->exec();
    }
    p->timerStop();
}

const QUuid &SchedulerTask::uuid() const
{
    return p->uuid;
}

bool SchedulerTask::start()
{
    QThread::start();
    while (this->eventDispatcher() == nullptr)
        QThread::msleep(1);
    return true;
}

}
