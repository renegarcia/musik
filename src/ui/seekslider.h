#ifndef SEEKSLIDER_H
#define SEEKSLIDER_H

#include <QSlider>


class SeekSlider : public QSlider
{
Q_OBJECT

public:
    SeekSlider( QWidget *parent = 0 );

public slots:
    void setTrackDuration( int ms );
    void setTrackPosition( int ms );

private:
    bool isPressed;

private slots:
    void sliderPressed();
    void sliderReleased();

signals:
    void seek( int ms );

};

#endif // SEEKSLIDER_H
