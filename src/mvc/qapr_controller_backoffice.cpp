#include "./qapr_controller_backoffice.h"
#include "../interfaces/qapr_interface.h"
#include "../../../qrpc/src/qrpc_controller.h"

namespace QApr {

QVector<QApr::Interface*> makeInterfaces(QObject*parent)
{
    QVector<QApr::Interface*> __return;
    auto&staticApiList=QRpc::Controller::staticApiList();
    for(auto&v:staticApiList){
        if(QApr::Interface::staticMetaObject.inherits(v))
            continue;
        auto&metaObject=*v;
        auto object=metaObject.newInstance(Q_ARG(QObject*, parent));
        if(object==nullptr)
            continue;
        auto parser=dynamic_cast<QApr::Interface*>(object);
        if(parser==nullptr){
            delete object;
            continue;
        }
        __return<<parser;
    }
    return __return;
}

static QVariantList&menuMaker(ControllerBackOffice*parent)
{
    static QVariantList rootMenuMaked;
    if(rootMenuMaked.isEmpty()){
        QHash<QString,QVariant> cacheMenu;
        auto makedInterfaces = makeInterfaces(parent);
        for(auto&v:makedInterfaces){
            auto vMenuList=v->backOfficeMenu();
            if(vMenuList.isEmpty())
                continue;

            for(auto&v:vMenuList){
                MenuObject menu(v);
                auto name=menu.text();
                if(menu.isEmpty())
                    continue;

                if(!cacheMenu.contains(name)){
                    cacheMenu[name]=menu.build();
                    continue;
                }

                MenuObject menuRoot(cacheMenu[name]);
                for(auto&v:menu.menu()){
                    MenuObject menuItem(v);
                    auto vMenu=v.toMap();
                    if(vMenu.isEmpty())
                        continue;
                    menuRoot.menu(v);
                }
                cacheMenu[name]=menuRoot.build();
            }
        }
        qDeleteAll(makedInterfaces);
        auto keys=cacheMenu.keys();
        keys.sort();
        for(auto&k:keys){
            MenuObject menu(cacheMenu[k]);
            rootMenuMaked<<menu.build();
        }
    }
    return rootMenuMaked;
}


ControllerBackOffice::ControllerBackOffice(QObject *parent) : QApr::Controller(parent)
{
}

ControllerBackOffice::~ControllerBackOffice()
{
}

ResultValue &ControllerBackOffice::sessionAccount()
{
    auto&credential=this->irq()->sessionObject().credential();
    auto vHash=credential.toVariant();
    return this->lr(vHash);
}

ResultValue &ControllerBackOffice::rootObject()
{
    QVariantHash vHash;
    vHash[qsl_fy(sessionAccount)]=this->sessionAccount().resultVariant();
    vHash[qsl_fy(menu)]=menuMaker(this);
    vHash[qsl_fy(enviroment)]=this->enviroment().resultVariant();
    return this->lr(vHash);
}

ResultValue &ControllerBackOffice::menuInfo(const QVariant &body)
{
    Q_UNUSED(body)
//    Q_DECLARE_VU;
//    auto vBody=vu.toHash(body);
//    auto uuid=vu.toUuid(vBody[qsl_fy(uuid)]);
//    auto name=vBody[qsl_fy(name)].toString();
//    Q_UNUSED(uuid)
//    Q_UNUSED(name)
    return this->lr(qvh_null);
}

ResultValue &ControllerBackOffice::enviroment()
{
    return this->lr(qvh{{qsl("version"), qsl("1.0.0")}});
}

}
