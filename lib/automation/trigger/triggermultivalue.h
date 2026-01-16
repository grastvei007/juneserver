#ifndef TRIGGERMULTIVALUE_H
#define TRIGGERMULTIVALUE_H

#include <map>
#include <memory>

#include <QJsonObject>

#include "../trigger.h"

class ValueItem;

class TriggerMultiValue : public TriggerBase
{
public:
    TriggerMultiValue(TagList &tagList, const QJsonObject &obj);

    QJsonObject toJson() const override;
    void update(const QJsonObject &obj) override;
    TriggerType type() const override;

protected:
    void tagSocketValueChanged(TagSocket *tagSocket) override;

private:
  bool isAllItemsTrue(const QStringList &words);
  bool isOneItemTrue(const QStringList &words);
  QString rule_;
  int duration_ = 0;
  bool triggerOnceADay_ = false;
  qint64 triggerTime_ = 0;
  std::map<QString, std::unique_ptr<ValueItem>> valueItems_;
};

#endif // TRIGGERMULTIVALUE_H
