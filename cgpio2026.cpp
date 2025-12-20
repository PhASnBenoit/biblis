#include "../biblis/cgpio2026.h"

#include <QDebug>
#include <stdexcept>

CGPIO2026::CGPIO2026(int gpio, Mode mode, QObject *parent)
    : QObject(parent),
    _gpio(gpio),
    _mode(mode),
    _chip(nullptr),
    _request(nullptr),
    _fd(-1),
    _notifier(nullptr)
{
    // Ouvrir le chip GPIO
    _chip = gpiod_chip_open("/dev/gpiochip0");
    if (!_chip)
        throw std::runtime_error("Impossible d'ouvrir /dev/gpiochip0");

    // Line settings
    gpiod_line_settings *settings = gpiod_line_settings_new();
    if (!settings)
        throw std::runtime_error("Impossible de cr�er gpiod_line_settings");

    if (_mode == Output) {
        gpiod_line_settings_set_direction(
            settings, GPIOD_LINE_DIRECTION_OUTPUT);
        gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);
    } else {
        gpiod_line_settings_set_direction(
            settings, GPIOD_LINE_DIRECTION_INPUT);
        gpiod_line_settings_set_edge_detection(
            settings, GPIOD_LINE_EDGE_BOTH);
    }

    // Line config
    gpiod_line_config *line_cfg = gpiod_line_config_new();
    unsigned int offsets[1] = {static_cast<unsigned int>(_gpio)};

    gpiod_line_config_add_line_settings(
        line_cfg, offsets, 1, settings);

    // Request config
    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "CGPIO2026");

    // Request line
    _request = gpiod_chip_request_lines(
        _chip, req_cfg, line_cfg);
    if (!_request)
        throw std::runtime_error("Impossible de demander la ligne GPIO"+std::to_string(_gpio));

    // Pour les entr�es : r�cup�rer le fd
    if (_mode == Input) {
        _fd = gpiod_line_request_get_fd(_request);
        if (_fd < 0)
            throw std::runtime_error("Impossible d'obtenir le fd GPIO");
    }

    // Lib�ration des configs
    gpiod_line_settings_free(settings);
    gpiod_line_config_free(line_cfg);
    gpiod_request_config_free(req_cfg);
}

CGPIO2026::~CGPIO2026()
{
    if (_notifier) {
        _notifier->setEnabled(false);
        delete _notifier;
    }

    if (_request)
        gpiod_line_request_release(_request);

    if (_chip)
        gpiod_chip_close(_chip);
}

void CGPIO2026::setValue(bool value)
{
    if (_mode != Output || !_request)
        return;

    gpiod_line_request_set_value(
        _request,
        _gpio,  // index dans la requ�te ATTENTION DOIT ETRE LE NUMERO DE LA GPIO
        value ? GPIOD_LINE_VALUE_ACTIVE
              : GPIOD_LINE_VALUE_INACTIVE
        );
}

bool CGPIO2026::value() const
{
    if (!_request)
        return false;

    return gpiod_line_request_get_value(
               _request, 0) == GPIOD_LINE_VALUE_ACTIVE;
}

void CGPIO2026::enableInterrupts()
{
    if (_mode != Input || _fd < 0)
        return;

    if (!_notifier) {
        _notifier = new QSocketNotifier(
            _fd, QSocketNotifier::Read, this);

        connect(_notifier,
                &QSocketNotifier::activated,
                this,
                &CGPIO2026::handleEvent);

        _notifier->setEnabled(true);
    }
}

void CGPIO2026::handleEvent()
{
    gpiod_edge_event_buffer *buffer =
        gpiod_edge_event_buffer_new(1);

    if (!buffer)
        return;

    int ret = gpiod_line_request_read_edge_events(
        _request, buffer, 1);

    if (ret > 0) {
        // ?? cast obligatoire � cause de l'API libgpiod
        gpiod_edge_event *event =
            (gpiod_edge_event *)
            gpiod_edge_event_buffer_get_event(buffer, 0);

        enum gpiod_edge_event_type type =
            gpiod_edge_event_get_event_type(event);

        if (type == GPIOD_EDGE_EVENT_RISING_EDGE)
            emit sig_risingEdge();
        else if (type == GPIOD_EDGE_EVENT_FALLING_EDGE)
            emit sig_fallingEdge();
    }

    gpiod_edge_event_buffer_free(buffer);
}
