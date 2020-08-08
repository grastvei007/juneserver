#include "logvaluemodel.h"

#include "logvaluedata.h"


LogValueTableModel::LogValueTableModel(LogValueData *aData, QObject *aParent) : QAbstractTableModel(aParent),
    mData(aData)
{
    if(mData)
        connect(mData, &LogValueData::logValueAdded, this, &LogValueTableModel::onLogValueAdded);
}

LogValueTableModel::~LogValueTableModel()
{

}

int LogValueTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    return mData->numberOfLogVAlues();
}

int LogValueTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 3;
}

QVariant LogValueTableModel::data(const QModelIndex &index, int role) const
{

    return QVariant(QVariant::Invalid);
}

QVariant LogValueTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole || role == Qt::EditRole)
        {
            switch (section) {
            case eTagName:
                return "Tag Name";
            case eTableName:
                return "Table Name";
            case eValueName:
                return "Value Name";
            default:
                break;
            }
        }
    }
    else if(orientation == Qt::Vertical)
    {
        if(role == Qt::DisplayRole || role == Qt::EditRole)
            return QString::number(section);
    }

    return QVariant(QVariant::Invalid);
}

Qt::ItemFlags LogValueTableModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsEnabled;
}

void LogValueTableModel::onLogValueAdded()
{

}

