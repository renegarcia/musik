#include "seekslider.h"


SeekSlider::SeekSlider( QWidget *parent )
        : QSlider( parent )
{
    setOrientation( Qt::Horizontal );
    isPressed = false;
    connect(this, SIGNAL(sliderPressed()),
            this, SLOT(sliderPressed()));
    connect(this, SIGNAL(sliderReleased()),
            this, SLOT(sliderReleased()));
}

void
SeekSlider::setTrackDuration( int ms )
{
    setMaximum( ms );
}

void
SeekSlider::setTrackPosition( int ms )
{
    if ( isPressed )
        return;
    setValue( ms );
}

void
SeekSlider::sliderPressed()
{
    isPressed = true;
}

void
SeekSlider::sliderReleased()
{
    isPressed = false;
    emit seek( value () );
}
