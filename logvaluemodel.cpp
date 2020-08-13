#include "logvaluemodel.h"

#include <QColor>

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
    if(role == Qt::DisplayRole)
    {
        const LogValue *value = mData->getLogValueByIndex(index.row());
        switch (index.column())
        {
            case eTableName:
            {
                return value->getTableName();
            }
            case eValueName:
            {
                return value->getValueNAme();
            }
            case eTagName:
            {
                QString tagname = QString("%1.%2").arg(value->getTagSubsystem()).arg(value->getTagName());
                return tagname;
            }
            default:
                Q_UNREACHABLE();
                break;
        }
    }
    else if(role == Qt::BackgroundRole)
    {
        if(index.row() == 0)
            return false;
        else if((index.row() % 2) == 1)
            return QColor(Qt::gray);
    }

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

void LogValueTableModel::insertLogVallue(const QString &aTable, const QString &aVAlueName, const QString &aTagSubsystem, const QString &aTagName)
{
    mData->addLogValue(aTable, aVAlueName, aTagSubsystem, aTagName);
    insertRows(1,1);
}

bool LogValueTableModel::insertRows(int row, int count, const QModelIndex &/*parent*/)
{
    auto idx = index(0, rowCount());
    beginInsertRows(idx, row, row + count);
    endInsertRows();
    return true;
}

Qt::ItemFlags LogValueTableModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsEnabled;
}

void LogValueTableModel::onLogValueAdded()
{
    emit layoutAboutToBeChanged();
    emit layoutChanged();
}

