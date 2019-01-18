#ifndef CLIENTINFORMATION_H
#define CLIENTINFORMATION_H

#include <QObject>
#include <QString>

class ClientInformation : public QObject
{
    Q_OBJECT
public:
    ClientInformation(QString aInfo, QObject *parent = 0);

    QString getName() const;
    QString getIp() const;

    bool hasError() const;
    QString errorStr() const;
signals:

public slots:

private:
    QString mName;
    QString mIp;

    bool mHasError;
    QString mErrorString;
};

#endif // CLIENTINFORMATION_H
