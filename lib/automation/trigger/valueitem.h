#ifndef VALUEITEM_H
#define VALUEITEM_H

#include <QJsonObject>
#include <QString>
#include <memory>
#include <optional>

#include <tagsystem/taglist.h>
#include <tagsystem/tagsocket.h>

enum class Operator { eLess, eAbove, eEqual, eNotEqual };

class ValueItem : public QObject
{
    Q_OBJECT
public:
    ValueItem(TagList &tagList, const QString &triggerName, const QJsonObject &item);

    const QString &name() const { return name_; }

    bool evaluate();

private slots:
    void onValueChanged(TagSocket *);

private:
    bool evaluateBool();
    bool evaluateInt();
    bool evaluateDouble();

    std::optional<Operator> stringToOperator(const QString &str);

    TagList &tagList_;

    QString name_;
    QString tagName_;

    std::shared_ptr<TagSocket> value_;
    QVariant targetValue_;
    QVariant currentValue_;
    std::optional<Operator> operator_;
};

#endif // VALUEITEM_H
