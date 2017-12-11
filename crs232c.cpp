#include "crs232c.h"

CRs232c::CRs232c(QObject *parent, const QString &nomPort)
{
    m_parent = parent;
    m_Sp = new QSerialPort(parent);
    m_Sp->setPortName(nomPort);
    connect(m_Sp, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_Sp, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(onErreur(QSerialPort::SerialPortError)));
    qDebug() << "L'objet CRs232c est créé par " << m_parent->thread();
}

CRs232c::~CRs232c()
{
    m_Sp->close();
    delete m_Sp;
    qDebug() << "L'objet CRs232c est détruit par " << m_parent->thread();
}

int CRs232c::initialiser(QSerialPort::BaudRate vitesse, QSerialPort::DataBits data,
                         QSerialPort::Parity parity, QSerialPort::StopBits nbStop,
                         QSerialPort::FlowControl flow)
{
    m_Sp->setBaudRate(vitesse);
    m_Sp->setDataBits(data);
    m_Sp->setParity(parity);
    m_Sp->setStopBits(nbStop);
    m_Sp->setFlowControl(flow);
    return 0;
}

int CRs232c::ouvrirPort()
{
    bool res=false;
    res=m_Sp->open(QIODevice::ReadWrite);
    if (!res) {
        m_Sp->close();
    } // if res
    return m_Sp->isOpen();
}

char CRs232c::ecrire(const char *trame, int nbOctets)
{
    int lg = m_Sp->write(trame, nbOctets);
    if ( lg < nbOctets) {
        return ERREUR;
    } // if erreur
    return lg;
}

void CRs232c::onReadyRead()
{
    QByteArray ba;
    ba = m_Sp->readAll();
    emit sigData(ba);
}

void CRs232c::onErreur(QSerialPort::SerialPortError err)
{
    emit sigErreur(err);
}
