#pragma once

#include "../../qstm/src/qstm_object_wrapper.h"
#include "./qapr_host.h"

namespace QApr {

//!
//! \brief The Settings class
//!
class Q_STM_EXPORT Settings : public QStm::ObjectWrapper
{
    Q_OBJECT
    QSTM_OBJECT_WRAPPER(Settings)
    Q_PROPERTY(Host *host READ host WRITE setHost RESET resetHost NOTIFY hostChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion RESET resetVersion NOTIFY versionChanged)
public:

    //!
    //! \brief Settings
    //! \param parent
    //!
    explicit Settings(QObject *parent = nullptr);

    //!
    //! \brief setValues
    //! \param v
    //! \return
    //!
    virtual bool setValues(const QVariant &v);

    //!
    //! \brief host
    //! \return
    //!
    Host *host();
    void setHost(Host *newHost);
    void resetHost();

    //!
    //! \brief version
    //! \return
    //!
    const QString &version() const;
    void setVersion(const QString &newVersion);
    void resetVersion();


private:
    Host _host;
    QString _version;
signals:
    void hostChanged();
    void versionChanged();
};

} // namespace QMFE
