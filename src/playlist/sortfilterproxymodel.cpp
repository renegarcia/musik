#include "sortfilterproxymodel.h"

SortFilterProxyModel::SortFilterProxyModel( QObject *parent )
        : QSortFilterProxyModel( parent )
{
}

void
SortFilterProxyModel::sort( int column, Qt::SortOrder order )
{
    sourceModel()->sort( column, order );
}
