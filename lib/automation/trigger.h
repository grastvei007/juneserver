#ifndef TRIGGER_H
#define TRIGGER_H

#include <QObject>
#include <QJsonObject>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>
#include <tagsystem/tagsocket.h>

enum class TriggerType
{
    TriggerEveryTimeAbove,
    TriggerEveryTimeBelow,
    TriggerOnTime,
    // set a tag value for a duration.
    ScheduleOnDuration,
    TriggerTwoValues
};

class TriggerBase : public QObject
{
    Q_OBJECT
public:
    explicit TriggerBase(TagList &tagList, const QJsonObject &obj, QObject *parent = nullptr);

    bool isActive() const;
    bool hasWatchTag() const;
    bool isEnabled() const;
    virtual bool shouldSave() const; // default is true

    Tag* watchTag() const;
    const QString& subsystem() const;
    const QString& triggerName() const;

    virtual QJsonObject toJson() const;
    virtual void update(const QJsonObject &obj);
    virtual TriggerType type() const = 0;

signals:
    void aboutToBeDestroyd(QString);

protected:
    virtual void tagSocketValueChanged(TagSocket *tagSocket) = 0;
    void setActive();
    void setDeactive();

    bool validateWatchTacksoket(TagSocket::Type type) const;

private slots:
    void onTagSocketValueChanged(TagSocket *tagSocket);

private:
    QString triggerTypeToString(TriggerType type) const;

    bool isActive_ = false;
    bool isEnabled_ = true;
    Tag *watchTag_ = nullptr;
    Tag *triggerTag_ = nullptr;
    TagList &tagList_;

    QString subsystem_;
    QString name_;
    QString triggerName_;

    TagSocket *watchTagSocket_ = nullptr;
};

#endif // TRIGGER_H
