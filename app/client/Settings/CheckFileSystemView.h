#ifndef CHECKFILESYSTEMVIEW_H
#define CHECKFILESYSTEMVIEW_H

#include "CheckFileSystemModel.h"

#include <QTreeView>

class CheckFileSystemView : public QTreeView
{
    Q_OBJECT
public:
    explicit CheckFileSystemView( QWidget* parent = 0 );

    void checkPath( const QString& path, Qt::CheckState state );
    void setExclusions( const QStringList& list );
    QStringList getExclusions();

signals:
    void dataChanged();

private:
    void fillDown( const QModelIndex& index);
    void updateParent( const QModelIndex& index );
    void getExclusionsForNode( const QModelIndex& index, QStringList&exclusions);

private slots:
    void onCollapse( const QModelIndex& idx );
    void onExpand( const QModelIndex& idx);
    void updateNode( const QModelIndex& idx);

private:
    CheckFileSystemModel* m_fsModel;
    QSet<qint64>  m_expandedSet;
};

#endif // CHECKFILESYSTEMVIEW_H
