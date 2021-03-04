#ifndef ARDUINO

#include "SerialDWM1001.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <chrono>
#include <thread>

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

void SerialDWM1001::write_tlv(
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
}

DWM1001Error SerialDWM1001::read_all_resp()
{
#ifdef DEBUG
    printf("read_all_resp:\n");
#endif
    clear_tlv_data();
    uint32_t tries = 0;
    uint32_t constexpr MAX_TRIES = 1000;
    while (check(sp_input_waiting(port)) == 0 && tries < MAX_TRIES) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        tries++;
    }

    uint8_t page = 0;
    while (true) {
        if (check(sp_input_waiting(port)) == 0) {
#ifdef DEBUG
            printf("read all input\n");
#endif
            return DWM1001Error::Ok;
        }
        auto data = last_tlv_data[page];
        check(sp_blocking_read(port, data, 2, timeout)); // type, length
        if (data[1] != 0) {
            check(sp_blocking_read(port, data + 2, data[1], timeout)); // value
        }
#ifdef DEBUG
        printf("read page %d: ", page);
        for (uint8_t i = 0; i < data[1] + 2; i++) {
            printf("%02x ", data[i]);
        }
        printf("\n");
#endif
        page++;
    }
}

sp_return SerialDWM1001::check_(sp_return result, char const* file, int line)
{
    if (result < 0) {
        printf("In %s at line %d:\n", file, line);
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

        default:
            printf("Unknown error.\n");
            break;
        }
    }
    return result;
}

#endif // ifndef ARDUINO
