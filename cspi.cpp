#include "cspi.h"

CSpi::CSpi(QObject *parent, char noCs, int speed) :
    QObject(parent)
{
    m_noCe = noCs;   // chip select
    m_speed = speed;
    init();
    qDebug() << "Démarrage de l'objet CSpi";
} // constructeur

/////////////////////////////////////////////////////////////////
int CSpi::lireNOctets(char *buffer, int n)
{
    return read(m_fileSpi, buffer, n);
} // lire

/////////////////////////////////////////////////////////////////
int CSpi::ecrireNOctets(quint8 *buffer, int n)
{
    int nb = write(m_fileSpi, buffer, n);
    if (nb !=n)
        emit sigErreur("CSpi::ecrireNOctets ERREUR écriture");
    return nb;
} // ecrire

/////////////////////////////////////////////////////////////////
int CSpi::init()
{
    char filename[20];

    sprintf(filename, "/dev/spidev0.%c", m_noCe);
    m_fileSpi=open(filename, O_RDWR);
    if(m_fileSpi==-1) {  // ouvre le fichier virtuel d'accès à SPI
        QString mess="CSpi::init Erreur ouverture acces au bus SPI";
        qDebug() << mess;
        emit sigErreur(mess);
        return -1;
    } // if open
    if (ioctl(m_fileSpi, SPI_IOC_WR_MAX_SPEED_HZ, & m_speed) != 0) {
        QString mess="CSpi::init Erreur ouverture acces au bus SPI";
         qDebug() << mess;
         emit sigErreur(mess);
         return -1;
    } // if
    return m_fileSpi;
} // init
