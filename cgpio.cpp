#include "cgpio.h"

CGpio::CGpio(int addr, int dir)
{
    mAddr = addr;
    gpioExport();
    init();
    gpioDirection(dir);
} // constructeur

CGpio::~CGpio()
{
  gpioUnexport();
} // destructeur

int CGpio::init()
{
    usleep(50000);
    sprintf(mFilename,"/sys/class/gpio/gpio%d/",mAddr);
    QString command = "sudo chmod -R 777 "+QString(mFilename);
    QProcess *proc= new QProcess(this);
    proc->start(command);
    sleep(2);
    return 1;
}

int CGpio::gpioUnexport()
{
    char buffer[3];

    QFile fUnexport("/sys/class/gpio/unexport");
    bool res = fUnexport.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!res) {
        qDebug() << "CGpio::gpioUnexport: Erreur d'ouverture du fichier !";
        emit erreur("CGpio::gpioUnexport: Erreur d'ouverture du fichier !");
        return -1;
    } // if erreur open
    sprintf(buffer,"%d", mAddr);
    int nbw = fUnexport.write(buffer, strlen(buffer));
    if (nbw != int(strlen(buffer))) {
        qDebug() << "CGpio::gpioUnexport: Erreur écriture dans fichier !";
        emit erreur("CGPIO::gpioUnexport: Erreur écriture dans fichier !");
        return -1;
    } // if nbw
    fUnexport.close();
    return 0;
}

int CGpio::gpioDirection(int dir)
{
    char buffer[3];
    QString ficDirection;

    ficDirection = QString("/sys/class/gpio/gpio%1/direction").arg(mAddr,0,10);
    QFile fDirection(ficDirection);
    bool res = fDirection.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!res) {
        qDebug() << "CGpio::gpioDirection: Erreur d'ouverture du fichier !";
        emit erreur("CGpio::gpioDirection: Erreur d'ouverture du fichier !");
        return -1;
    } // if erreur open
    if (dir==IN)
      strcpy(buffer,"in");
    else
      strcpy(buffer,"out");
    int nbw = fDirection.write(buffer, strlen(buffer));
    if (nbw != int(strlen(buffer))) {
        qDebug() << "CGpio::gpioDirection: Erreur écriture dans fichier !";
        emit erreur("CGPIO::gpioDirection: Erreur écriture dans fichier !");
        return -1;
    } // if nbw
    fDirection.close();
    return 0;
}

int CGpio::gpioExport()
{
    char buffer[3];

    QFile fExport("/sys/class/gpio/export");
    bool res = fExport.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!res) {
        qDebug() << "CGpio::gpioExport: Erreur d'ouverture du fichier !";
        emit erreur("CGpio::gpioExport: Erreur d'ouverture du fichier !");
        return -1;
    } // if erreur open
    sprintf(buffer,"%d", mAddr);
    int nbw = fExport.write(buffer, strlen(buffer));
    if (nbw != int(strlen(buffer))) {
        qDebug() << "CGpio::gpioExport: Erreur écriture dans fichier !";
        emit erreur("CGPIO::gpioExport: Erreur écriture dans fichier !");
        return -1;
    } // if nbw
    fExport.close();
    return 0;
}
	

int CGpio::lire()
{
    char buffer[3];
    QString ficValue;

    ficValue = QString("/sys/class/gpio/gpio%1/value").arg(mAddr,0,10);
    QFile fValue(ficValue);
    bool res = fValue.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!res) {
        qDebug() << "CGpio::gpioLire: Erreur d'ouverture du fichier !";
        emit erreur("CGpio::gpioLire: Erreur d'ouverture du fichier !");
        return -1;
    } // if erreur open

    int nbr = fValue.read(buffer, sizeof(buffer));
    if (nbr == -1) {
        qDebug() << "CGpio::gpioLire: Erreur lecture dans fichier !";
        emit erreur("CGPIO::gpioLire: Erreur lecture dans fichier !");
        return -1;
    } // if nbw
    buffer[nbr]=0;  // car NULL
    fValue.close();
    return atoi(buffer);
}
	
int CGpio::ecrire(int value)
{
    char buffer[3]={'0', '1'};
    QString ficValue;

    ficValue = QString("/sys/class/gpio/gpio%1/value").arg(mAddr,0,10);
    QFile fValue(ficValue);
    bool res = fValue.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!res) {
        qDebug() << "CGpio::gpioEcrire: Erreur d'ouverture du fichier !";
        emit erreur("CGpio::gpioEcrire: Erreur d'ouverture du fichier !");
        return -1;
    } // if erreur open

    int nbw = fValue.write(&buffer[(value==0?0:1)], 1);
    if (nbw == -1) {
        qDebug() << "CGpio::gpioEcrire: Erreur écriture dans fichier !";
        emit erreur("CGPIO::gpioEcrire: Erreur écriture dans fichier !");
        return -1;
    } // if nbw
    fValue.close();
    return 0;

}	
