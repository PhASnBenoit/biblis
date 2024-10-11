// PhA @2024
#ifndef CGPIO2024_H
#define CGPIO2024_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>

class CGpio2024 : public QObject
{
    Q_OBJECT
public:
    explicit CGpio2024(QObject *parent = nullptr, int noGpio = 27, QString compo = "gpiochip0");
    ~CGpio2024();
    void setToOn();
    void setToOff();
    int getGpio();

private:
    int _noGpio;
    QString _compo;
    QString _command;
    QStringList _args;
    void setTo(int state);

signals:

};

#endif // CGPIO2024_H
