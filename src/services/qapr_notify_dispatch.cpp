#include "./qapr_notify_dispatch.h"
#include "./qapr_notify.h"
#include "../../../qstm/src/qstm_util_variant.h"

namespace QApr {

class NotifyDispatchPvt:public QObject
{
public:
    QByteArray channel;
    QByteArray md5;
    NotifyDispatch*parent=nullptr;
    VariantUtil vu;
    explicit NotifyDispatchPvt(NotifyDispatch*parent=nullptr):QObject{parent}
    {
        this->parent=parent;
    }
};

NotifyDispatch::NotifyDispatch(QObject *parent):QObject{parent}
{
    Q_UNUSED(parent)
    this->p= new NotifyDispatchPvt{this};
}

NotifyDispatch::NotifyDispatch(const QByteArray &name, const QByteArray &md5, QObject *parent):QObject{parent}
{
    Q_UNUSED(parent)
    this->p= new NotifyDispatchPvt{this};
    p->channel=name;
    p->md5=md5;
}


void NotifyDispatch::clear()
{
    this->setChannel({});
    this->setMd5({});
}

QByteArray NotifyDispatch::channel() const
{
    return p->channel;
}

void NotifyDispatch::setChannel(const QByteArray &value)
{
    p->channel=value;
}

QByteArray NotifyDispatch::md5() const
{
    return p->md5;
}

void NotifyDispatch::setMd5(const QByteArray &value)
{
    p->md5=value;
}

void NotifyDispatch::notify(const QVariant &payload)
{
    Notify::instance().notify(this->channel(), payload);
}

void NotifyDispatch::notify(const QString &channel, const QVariant &payload)
{
    Notify::instance().notify(channel, payload);
}

}

