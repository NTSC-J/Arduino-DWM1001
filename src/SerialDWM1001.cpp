#ifndef ARDUINO

#include "SerialDWM1001.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

SerialDWM1001::SerialDWM1001(char const *const portname, int32_t timeout) :
    timeout(timeout)
{
    check(sp_get_port_by_name(portname, &port));
    check(sp_open(port, SP_MODE_READ_WRITE));
    check(sp_set_baudrate(port, 115200));
    check(sp_set_bits(port, 8));
    check(sp_set_parity(port, SP_PARITY_NONE));
    check(sp_set_stopbits(port, 1));
    check(sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE));

#ifdef DEBUG
    printf("Port name: %s\n", sp_get_port_name(port));
    printf("Description: %s\n", sp_get_port_description(port));
    
    sp_transport transport = sp_get_port_transport(port);
    if (transport == SP_TRANSPORT_NATIVE) {
            printf("Type: Native\n");
    } else if (transport == SP_TRANSPORT_USB) {
            printf("Type: USB\n");

            printf("Manufacturer: %s\n", sp_get_port_usb_manufacturer(port));
            printf("Product: %s\n", sp_get_port_usb_product(port));
            printf("Serial: %s\n", sp_get_port_usb_serial(port));

            int usb_vid, usb_pid;
            sp_get_port_usb_vid_pid(port, &usb_vid, &usb_pid);
            printf("VID: %04X PID: %04X\n", usb_vid, usb_pid);

            int usb_bus, usb_address;
            sp_get_port_usb_bus_address(port, &usb_bus, &usb_address);
            printf("Bus: %d Address: %d\n", usb_bus, usb_address);
    } else if (transport == SP_TRANSPORT_BLUETOOTH) {
            printf("Type: Bluetooth\n");
            printf("MAC: %s\n", sp_get_port_bluetooth_address(port));
    }

    struct sp_port_config *initial_config;
    check(sp_new_config(&initial_config));
    check(sp_get_config(port, initial_config));

    int baudrate, bits, stopbits;
    enum sp_parity parity;
    check(sp_get_config_baudrate(initial_config, &baudrate));
    check(sp_get_config_bits(initial_config, &bits));
    check(sp_get_config_stopbits(initial_config, &stopbits));
    check(sp_get_config_parity(initial_config, &parity));
    printf("Baudrate: %d, data bits: %d, parity: %d, stop bits: %d\n",
                    baudrate, bits, parity, stopbits);
#endif
}

SerialDWM1001::~SerialDWM1001()
{
#ifdef DEBUG
    printf("Closing port\n");
#endif
    check(sp_close(port));
}

DWM1001Error SerialDWM1001::write_tlv(
        uint8_t const type, uint8_t const length, uint8_t const* const value)
{
#ifdef DEBUG
    printf("write_tlv: type = 0x%02x, length = %u, value = ", type, length);
    for (uint8_t i = 0; i < length; i++) {
        printf("%02x ", value[i]);
    }
    printf("\n");
#endif
    uint8_t buf[256];
    buf[0] = type;
    buf[1] = length;
    memcpy(buf + 2, value, length);

    assert(length + 2 == check(sp_blocking_write(port, buf, length + 2, timeout)));
    return DWM1001Error::Ok;
}

DWM1001Error SerialDWM1001::read_tlv(
        uint8_t *const type, uint8_t *const length, uint8_t *const value)
{
    // need to do this because the 2nd argument of sp_blocking_read
    // doesn't have the const qualifier
    {
        uint8_t type_;
        check(sp_blocking_read(port, &type_, 1, timeout));
        *type = type_;
    }
#ifdef DEBUG
    printf("read_tlv: type = 0x%02x, ", *type);
#endif
    {
        uint8_t length_;
        check(sp_blocking_read(port, &length_, 1, timeout));
        *length = length_;
    }
#ifdef DEBUG
    printf("length = %u, value = ", *length);
#endif
    {
        uint8_t value_[256];
        check(sp_blocking_read(port, value_, *length, timeout));
        memcpy(value, value_, *length);
    }
#ifdef DEBUG
    for (uint8_t i = 0; i < *length; i++) {
        printf("%02x ", value[i]);
    }
    printf("\n");
#endif

    return DWM1001Error::Ok;
}

sp_return SerialDWM1001::check(sp_return result)
{
    switch (result) {
    case SP_ERR_ARG:
        printf("Error: Invalid argument.\n");
        abort();

    case SP_ERR_FAIL: {
        char *error_message = sp_last_error_message();
        printf("Error: Failed: %s\n", error_message);
        sp_free_error_message(error_message);
        abort();
    }

    case SP_ERR_SUPP:
        printf("Error: Not supported.\n");
        abort();

    case SP_ERR_MEM:
        printf("Error: Couldn't allocate memory.\n");
        abort();

    case SP_OK:
    default:
        break;
    }
    return result;
}

#endif // ifndef ARDUINO
