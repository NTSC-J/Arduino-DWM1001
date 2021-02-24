#ifndef ARDUINO

#include "SerialDWM1001.h"
#include <cstdio>

int main()
{
    printf("Initializing port\n");
    auto dev = SerialDWM1001("/dev/ttyACM0");

    printf("\npos_get\n");
    auto pos = Position();
    dev.pos_get(&pos);
    printf("pos: (%d, %d, %d), qf = %d\n", pos.x, pos.y, pos.z, pos.qf);

    printf("\nupd_rate_get\n");
    uint16_t ur, urs;
    dev.upd_rate_get(&ur, &urs);
    printf("ur = %u, urs = %u\n", ur, urs);

    printf("\ngpio_cfg_output\n"); // LED D12
    dev.gpio_cfg_output(GPIOIdx::GPIO14, true);

    printf("\npanid_get\n");
    uint16_t panid;
    dev.panid_get(&panid);
    printf("panid: 0x%04x\n", panid);

    printf("\nnodeid_get\n");
    uint64_t nodeid;
    dev.nodeid_get(&nodeid);
    printf("nodeid: 0x%016lx\n", nodeid);

    for (int i = 0; i < 2; i++) {
        printf("gpio_value_toggle\n");
        dev.gpio_value_toggle(GPIOIdx::GPIO14);
        bool v;
        dev.gpio_value_get(GPIOIdx::GPIO14, &v);
        printf("gpio_value_get: %d\n", v);
    }

    return 0;
}

#endif // ifndef ARDUINO
