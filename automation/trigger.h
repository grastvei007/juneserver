#ifndef TRIGGER_H
#define TRIGGER_H

#include <QObject>
#include <QJsonObject>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>
#include <tagsystem/tagsocket.h>

enum class RuleType
{
    TriggerEveryTimeAbove,
    TriggerEveryTimeBelow,
    TriggerOnTime
};

class TriggerBase : public QObject
{
    Q_OBJECT
public:
    explicit TriggerBase(TagList &tagList, const QJsonObject &obj, QObject *parent = nullptr);

    bool isActive() const;
    bool hasWatchTag() const;

    Tag* watchTag() const;
    const QString& subsystem() const;
    const QString& triggerName() const;

    virtual QJsonObject toJson() const;

protected:
    virtual void tagSocketValueChanged(TagSocket *tagSocket) = 0;
    void setActive();
    void setDeactive();

private slots:
    void onTagSocketValueChanged(TagSocket *tagSocket);

private:
    bool isActive_ = false;
    Tag *watchTag_ = nullptr;
    Tag *triggerTag_ = nullptr;
    TagList &tagList_;

    QString subsystem_;
    QString name_;
    QString triggerName_;

    TagSocket *watchTagSocket_ = nullptr;
};

#endif // TRIGGER_H
