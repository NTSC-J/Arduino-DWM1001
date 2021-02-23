#ifndef ARDUINO

#include "SerialDWM1001.h"
#include <cstdio>

int main()
{
    printf("Initializing port\n");
    auto dev = SerialDWM1001("/dev/ttyACM0");

    printf("\nreset\n");
    dev.reset();

    printf("\npos_get\n");
    auto newpos = Position();
    dev.pos_get(&newpos);
    printf("pos: (%d, %d, %d)\n", newpos.x, newpos.y, newpos.z);

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

    return 0;
}

#endif // ifndef ARDUINO
