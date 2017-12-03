#ifndef CSPI_H
#define CSPI_H

#include <QObject>
#include <QDebug>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

class CSpi : public QObject
{
    Q_OBJECT

public:
    explicit CSpi(QObject *parent = 0, char noCs = '0', int speed=7000000);
    int lireNOctets(char *buffer, int n);
    int ecrireNOctets(quint8 *buffer, int n);

private:
    int init();
    //int m_addr;   // Adresse du composant SPI
    char m_noCe;   // No du device CE/
    int m_speed;   // vitesse du bus SPI pour CE0
    int m_fileSpi;  // descripteur du fichier Spi

signals:
    void sigErreur(QString msg);
};

#endif // CSPI_H
