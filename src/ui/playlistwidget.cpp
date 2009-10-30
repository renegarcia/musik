#include "playlistwidget.h"
#include <QGridLayout>
#include <QModelIndex>
#include <QLabel>
#include <QFileDialog>

PlaylistWidget::PlaylistWidget( QWidget *parent, Xmms::Client *xmms )
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
    filter->setVisible( false );

    jumpLabel = new QLabel( tr("&Jump"), this );
    jumpLabel->setBuddy( filter );
    jumpLabel->setVisible( false );

    setCore( new CorePlaylist( this, xmms ) );

    QGridLayout *layout = new QGridLayout( this );
    layout->addWidget( jumpLabel, 0, 0);
    layout->addWidget( filter, 0, 1);
    layout->addWidget( view, 1, 0, 1, 2 );
    this->setLayout( layout );
    
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

    connect ( filter, SIGNAL(textChanged(QString)),
              proxyModel, SLOT(setFilterWildcard(QString)));

    connect( view, SIGNAL(activated(QModelIndex)),
             this, SLOT(play(QModelIndex)));
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

void
PlaylistWidget::setFilterVisible( bool ok )
{
    if ( ok ){
        jumpLabel->show();
        filter->show();
        QString text = filter->text();
        proxyModel->setFilterWildcard( text );
        filter->setFocus();
    }
    else{
        proxyModel->setFilterWildcard( "" );
        jumpLabel->hide();
        filter->hide();
    }
}

void
PlaylistWidget::addMedia()
{
    QList< QString > files = QFileDialog::getOpenFileNames( this,
                                                    tr("Select media"));
    foreach ( QString absName, files ){
        core->add( "file://" + absName );
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
