#include "CheckFileSystemModel.h"

CheckFileSystemModel::CheckFileSystemModel( QObject *parent )
    :QFileSystemModel( parent )
{
}

Qt::ItemFlags
CheckFileSystemModel::flags( const QModelIndex& index) const
{
    return QFileSystemModel::flags(index) | Qt::ItemIsUserCheckable;
}

QVariant
CheckFileSystemModel::data( const QModelIndex& index, int role) const
{
    if (role == Qt::CheckStateRole)
    {
        qint64 id = index.internalId();
        return m_checkTable.contains(id) ? m_checkTable.value(id) : Qt::Unchecked;
    }
    else
    {
        return QFileSystemModel::data(index, role);
    }
}

bool
CheckFileSystemModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if (role == Qt::CheckStateRole)
    {
        m_checkTable.insert(index.internalId(), (Qt::CheckState)value.toInt());

        emit dataChanged(index, index);
        emit dataChangedByUser(index);

        return true;
    }
    else
    {
        return QFileSystemModel::setData(index, value, role);
    }
}

void
CheckFileSystemModel::setCheck( const QModelIndex& index, const QVariant& value)
{
    m_checkTable.insert(index.internalId(), (Qt::CheckState)value.toInt());
    emit dataChanged(index, index);
}

Qt::CheckState
CheckFileSystemModel::getCheck( const QModelIndex& index )
{
    return (Qt::CheckState)data(index, Qt::CheckStateRole).toInt();
}
