#include "playlistwidget.h"
#include <QBoxLayout>
#include <QModelIndex>

PlaylistWidget::PlaylistWidget( QWidget *parent, CorePlaylist *core )
        : QWidget(parent)
{
    proxyModel = 0;
    model = 0;

    view = new QTreeView( this );
    view->setSelectionBehavior( QAbstractItemView::SelectRows );
    view->setAlternatingRowColors( true );
    view->setRootIsDecorated( false );
    view->setSelectionMode( QAbstractItemView::ExtendedSelection );
    view->setDragEnabled( true );
    view->setAcceptDrops( true );
    view->setDropIndicatorShown( true );

    filter = new KLineEdit( this );
    filter->setClearButtonShown( true );

    this->setLayout( new QBoxLayout( QBoxLayout::TopToBottom, this ) );
    this->layout()->addWidget( view );
    this->layout()->addWidget( filter );

    if ( core )
        setCore( core );
}

void
PlaylistWidget::setCore( CorePlaylist *core )
{
    this->core = core;

    if ( !model )
        model = new MediaTableModel( this, core );
    else
        model->setCore( core );

    if ( !proxyModel )
        proxyModel = new SortFilterProxyModel( this );

    proxyModel->setSourceModel( model );
    proxyModel->setFilterCaseSensitivity( Qt::CaseInsensitive );
    view->setModel( proxyModel );
    proxyModel->setFilterKeyColumn( -1 );
    view->sortByColumn( -1 );
    view->setSortingEnabled( true );


    connect ( filter, SIGNAL(textChanged(QString)), proxyModel,
                  SLOT(setFilterWildcard(QString)));

    connect( view, SIGNAL(activated(QModelIndex)), this,
             SLOT(play(QModelIndex)));
}

/**
  *Public slots
  */
void
PlaylistWidget::viewPlaylist( const QString &name )
{
    core->setPlaylist( name );
}

void
PlaylistWidget::removeSelection()
{
    QModelIndexList rowList = view->selectionModel()->selectedRows();
    int pos;

    for ( int i = rowList.size() - 1; i > -1; i--){
        pos = proxyModel->mapToSource( rowList.at( i ) ).row();
        core->remove( pos );
    }
}

//private slots
void
PlaylistWidget::play( const QModelIndex index )
{
    int pos = proxyModel->mapToSource( index ).row();
    core->activate( pos );
    emit play();
}
