#pragma once

#include <QStm>

namespace QApr {
class HostPvt;
class Q_STM_EXPORT Host : public QStm::ObjectWrapper
{
    Q_OBJECT
    QSTM_OBJECT_WRAPPER(Host)
    Q_PROPERTY(QByteArray protocol READ protocol WRITE setProtocol RESET resetProtocol NOTIFY protocolChanged)

    Q_PROPERTY(QByteArray hostName READ hostName WRITE setHostName RESET resetHostName NOTIFY hostNameChanged)

    Q_PROPERTY(int port READ port WRITE setPort RESET resetPort NOTIFY portChanged)

    Q_PROPERTY(QVariantHash headers READ headers WRITE setHeaders RESET resetHeaders NOTIFY headersChanged)

    Q_PROPERTY(QByteArray basePath READ basePath WRITE setBasePath RESET resetBasePath NOTIFY basePathChanged)
public:
    Q_INVOKABLE explicit Host(QObject *parent = nullptr);

    ~Host();

    //!
    //! \brief isValid
    //! \return
    //!
    Q_INVOKABLE bool isValid()const;

    //!
    //! \brief isEmpty
    //! \return
    //!
    Q_INVOKABLE bool isEmpty()const;

    const QByteArray &protocol() const;
    void setProtocol(const QByteArray &newProtocol);
    void resetProtocol();

    const QByteArray &hostName() const;
    void setHostName(const QByteArray &newHostName);
    void resetHostName();

    int port() const;
    void setPort(int newPort);
    void resetPort();

    QVariantHash &headers()const;
    void setHeaders(const QVariantHash &newHeaders);
    void addHeaders(const QVariantHash &newHeaders);
    void resetHeaders();

    const QByteArray &basePath() const;
    void setBasePath(const QByteArray &newBasePath);
    void resetBasePath();

private:
    HostPvt *p=nullptr;
signals:

    void protocolChanged();
    void hostNameChanged();
    void portChanged();
    void headersChanged();
    void basePathChanged();
};

} // namespace QMFE