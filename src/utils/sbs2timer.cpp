#include "sbs2timer.h"

qint64 Sbs2Timer::tic_time = 0;
QString Sbs2Timer::label = "";

Sbs2Timer::Sbs2Timer(QObject *parent) :
    QObject(parent)
{
}

void Sbs2Timer::tic(QString label_)
{
    label = label_;
    tic_time = QDateTime::currentMSecsSinceEpoch();
}

void Sbs2Timer::tic()
{
    tic(label);
}

void Sbs2Timer::toc()
{
    qDebug() << (label.toStdString().c_str()) <<(QDateTime::currentMSecsSinceEpoch() - tic_time);
    tic_time = 0;
    label = "";
}
