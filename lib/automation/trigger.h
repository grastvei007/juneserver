#ifndef TRIGGER_H
#define TRIGGER_H

#include <vector>

#include <QJsonArray>
#include <QJsonObject>
#include <QObject>

#include <tagsystem/tag.h>
#include <tagsystem/taglist.h>
#include <tagsystem/tagsocket.h>
#include <tagsystem/util/date.h>

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
	bool shouldTriggerToday() const;
	TagList &tagList() { return tagList_; }

  private slots:
	void onTagSocketValueChanged(TagSocket *tagSocket);

private:
  void parseArrayWithDays(const QJsonArray &days);
  void parseArrayWithMonths(const QJsonArray &months);

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

  std::vector<util::date::DayOfWeek> triggerOnTheseDays_; //< If empty trigger on all days
  std::vector<util::date::Month> triggerInTheseMonths_;   ///< active months
};

#endif // TRIGGER_H
