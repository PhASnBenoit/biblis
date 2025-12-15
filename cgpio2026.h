#pragma once

#include <QObject>
#include <QSocketNotifier>
#include <QDebug>
#include <gpiod.h>

class CGPIO2026 : public QObject
{
    Q_OBJECT
public:
    enum Mode { Input, Output };

    explicit CGPIO2026(int gpio, Mode mode, QObject *parent = nullptr);
    ~CGPIO2026();

    void setValue(bool value);
    bool value() const;

    void enableInterrupts();

signals:
    void sig_risingEdge();
    void sig_fallingEdge();

private slots:
    void handleEvent();

private:
    int _gpio;
    Mode _mode;

    gpiod_chip *_chip;
    gpiod_line_request *_request;
    int _fd;  // File descriptor de la ligne
    QSocketNotifier *_notifier;
};
