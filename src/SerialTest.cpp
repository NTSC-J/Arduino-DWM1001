#ifndef ARDUINO

#include "SerialDWM1001.h"
#include <cstdio>

int main()
{
    printf("Initializing port\n");
    auto dev = SerialDWM1001("/dev/ttyACM0");

    printf("nop\n");
    dev.nop();

    printf("reset\n");
    dev.reset();

/*
    printf("pos_set\n");
    dev.pos_set({1, 1, 1});
*/
    printf("pos_get\n");
    auto newpos = Position();
    dev.pos_get(&newpos);
    printf("newpos: %d %d %d\n", newpos.x, newpos.y, newpos.z);

    printf("upd_rate_get\n");
    uint16_t ur, urs;
    dev.upd_rate_get(&ur, &urs);
    printf("ur = %u, urs = %u\n", ur, urs);

/*
    printf("upd_rate_set\n");
    dev.upd_rate_set(1, 5);
*/

    return 0;
}

#endif // ifndef ARDUINO
