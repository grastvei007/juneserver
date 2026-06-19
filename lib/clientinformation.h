#ifndef CLIENTINFORMATION_H
#define CLIENTINFORMATION_H

#include <QObject>
#include <QString>

class ClientInformation : public QObject
{
    Q_OBJECT
public:
	ClientInformation(QString info, QObject *parent = 0);

    QString getName() const;
    QString getIp() const;
	QStringList getTagSet() const;
	bool hasTagSet() const;

    bool hasError() const;
    QString errorStr() const;
signals:

public slots:

private:
	QString name_;
	QString ip_;
	QStringList tagSet_;

	bool hasError_ = false;
	QString errorString_;
};

#endif // CLIENTINFORMATION_H
