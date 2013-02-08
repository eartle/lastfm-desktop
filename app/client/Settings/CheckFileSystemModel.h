#ifndef CHECKFILESYSTEMMODEL_H
#define CHECKFILESYSTEMMODEL_H

#include <QFileSystemModel>

class CheckFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit CheckFileSystemModel( QObject *parent = 0 );
    
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    void setCheck( const QModelIndex& index, const QVariant& value);
    Qt::CheckState getCheck( const QModelIndex& index );

signals:
    void dataChangedByUser( const QModelIndex & index);

private:
    QHash<qint64, Qt::CheckState> m_checkTable;
};

#endif // CHECKFILESYSTEMMODEL_H
