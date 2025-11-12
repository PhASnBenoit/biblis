#include "cgpio2024.h"
// changement de la bibliothèque pour les nouvelles versions de RPI OS
CGpio2024::CGpio2024(QObject *parent, int noGpio, QString compo)
    : QObject{parent}
{
    _noGpio = noGpio;
    _compo = compo;
}

CGpio2024::~CGpio2024()
{
}

void CGpio2024::setToOn()
{
    setTo(1);
}

void CGpio2024::setToOff()
{
    setTo(0);
}

// -1 erreur, 0 ou 1 état gpio
int CGpio2024::getGpio()
{
    QProcess *p;

    _command = "gpioget";
    _args.clear();
    _args << _compo;
    _args << QString::number(_noGpio);
    p = new QProcess(this);
    p->start(_command, _args);
    if (!p->waitForFinished(500)) {
        delete p;
        return -1;
    }
    int ret = p->readAllStandardOutput().toInt();
    delete p;
    return ret;
}

void CGpio2024::setTo(int state)
{
    QProcess *p;

    _command = "gpioset";
    _args.clear();
    _args << _compo;
    _args << QString::number(_noGpio)+"="+QString::number(state);
    p = new QProcess(this);
    p->start(_command, _args);
    if (!p->waitForFinished(500)) {
        delete p;
        return;
    }
    delete p;
}
