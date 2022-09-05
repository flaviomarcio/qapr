#include "./qapr_session_credential.h"
#include "../../../qstm/src/qstm_util_variant.h"

namespace QApr {

class CredentialsPvt:public QObject{
public:
    SessionCredential *parent=nullptr;
    QUuid uuid;
    QVariantHash permits;
    QUuid account_uuid;
    QUuid domain_uuid;
    QUuid session_uuid;
    QUuid service_uuid;
    QVariant profile;
    QVariant session;
    QVariant token;
    explicit CredentialsPvt(SessionCredential*parent):QObject{parent}
    {
        this->parent=parent;
    }
};


SessionCredential::SessionCredential(QObject *parent) : QObject{parent}
{
    this->p=new CredentialsPvt{this};
}

QVariantHash SessionCredential::toVariant() const
{
    QVariantHash vHash{
         {QT_STRINGIFY(session_uuid), this->sessionUuid()}
        ,{QT_STRINGIFY(account_uuid), this->accountUuid()}
        ,{QT_STRINGIFY(profile), this->profile()}
        ,{QT_STRINGIFY(session), this->session()}
        ,{QT_STRINGIFY(permits), this->permits()}
    };
    return vHash;
}

bool SessionCredential::isValid() const
{
    auto &credentials=*this;
    if(credentials.serviceUuid().isNull() && credentials.sessionUuid().isNull())
        return {};

    return true;
}

QVariantHash &SessionCredential::permits()const
{
    return p->permits;
}

SessionCredential &SessionCredential::setPermits(const QVariantHash &permits)
{
    p->permits = permits;
    return *this;
}

QUuid &SessionCredential::uuid() const
{
    return p->uuid;
}

SessionCredential &SessionCredential::setUuid(const QUuid &value)
{
    p->uuid = value;
    return *this;
}

QUuid SessionCredential::domainUuid() const
{
    return p->domain_uuid;
}

SessionCredential &SessionCredential::setDomainUuid(const QVariant &uuid)
{
    Q_DECLARE_VU;
    p->domain_uuid=vu.toUuid(uuid);
    return *this;
}

QUuid SessionCredential::accountUuid() const
{
    return p->account_uuid;
}

SessionCredential &SessionCredential::set_account_uuid(const QVariant &uuid)
{
    Q_DECLARE_VU;
    p->account_uuid=vu.toUuid(uuid);
    return *this;
}

QUuid SessionCredential::sessionUuid() const
{
    Q_DECLARE_VU;
    return p->session_uuid;
}

SessionCredential &SessionCredential::setSessionUuid(const QVariant &uuid)
{
    Q_DECLARE_VU;
    p->session_uuid=vu.toUuid(uuid);
    return *this;
}

QUuid SessionCredential::serviceUuid() const
{
    Q_DECLARE_VU;
    return p->service_uuid;
}

SessionCredential &SessionCredential::setServiceUuid(const QVariant &uuid)
{
    Q_DECLARE_VU;
    p->service_uuid=vu.toUuid(uuid);
    return *this;
}

QVariant SessionCredential::profile() const
{
    return p->profile;
}

SessionCredential &SessionCredential::setProfile(const QVariant &value)
{
    p->profile=value;
    return *this;
}

QVariant SessionCredential::session() const
{
    return p->session;
}

SessionCredential &SessionCredential::setSession(const QVariant &value)
{
    p->session=value;
    return *this;
}

QVariant SessionCredential::token() const
{
    return p->token;
}

SessionCredential &SessionCredential::setToken(const QVariant &value)
{
    p->token=value;
    return *this;
}

}
