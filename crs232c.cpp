#include "crs232c.h"

CRs232c::CRs232c(QObject *parent, const QString &nomPort)
{
  mSp = new QSerialPort(parent);
  mSp->setPortName(nomPort);
  connect(mSp, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
  connect(mSp, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(onErreur(QSerialPort::SerialPortError)));
}

CRs232c::~CRs232c()
{
    mSp->close();
    delete mSp;
}

int CRs232c::initialiser(QSerialPort::BaudRate vitesse, QSerialPort::DataBits data,
                         QSerialPort::Parity parity, QSerialPort::StopBits nbStop,
                         QSerialPort::FlowControl flow)
{
    mSp->setBaudRate(vitesse);
    mSp->setDataBits(data);
    mSp->setParity(parity);
    mSp->setStopBits(nbStop);
    mSp->setFlowControl(flow);
    return 0;
}

int CRs232c::ouvrirPort()
{
    bool res=false;
    res=mSp->open(QIODevice::ReadWrite);
    if (!res) {
        mSp->close();
    } // if res
    return mSp->isOpen();
}

char CRs232c::ecrire(char *trame, int nbOctets)
{
    mRec = true; // les futurs cars arrivant sont valides
    int lg = mSp->write(trame, nbOctets);
    if ( lg < nbOctets) {
        return ERREUR;
    } // if erreur
    return OK;
}

void CRs232c::onReadyRead()
{
    QByteArray ba;
    ba = mSp->readAll();
    emit sigData(ba);
}

void CRs232c::onErreur(QSerialPort::SerialPortError err)
{
    emit sigErreur(err);
}
