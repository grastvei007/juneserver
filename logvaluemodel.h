#ifndef LOGVALUEMODEL_H
#define LOGVALUEMODEL_H

#include <QAbstractTableModel>

class LogValueData;

class LogValueTableModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Columns
    {
        eTableName,
        eValueName,
        eTagName
    };

    LogValueTableModel(LogValueData *amData, QObject *aParent = nullptr);
    ~LogValueTableModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

private slots:
    void onLogValueAdded();

private:
    LogValueData *mData;
};

#endif // LOGVALUEMODEL_H
