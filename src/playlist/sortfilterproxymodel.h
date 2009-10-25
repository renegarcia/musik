#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class SortFilterProxyModel : public QSortFilterProxyModel
{
public:
    SortFilterProxyModel( QObject *parent );

    void sort( int column, Qt::SortOrder order );
};

#endif // SORTFILTERPROXYMODEL_H
