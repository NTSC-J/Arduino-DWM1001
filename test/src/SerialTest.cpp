#include "SerialDWM1001.h"
#include <cstdio>
#include <thread>
#include <chrono>

void dump_status(Status const& status)
{
    printf("loc_ready: %d\n", status.loc_ready);
    printf("uwbmac_joined: %d\n", status.uwbmac_joined);
    printf("bh_data_ready: %d\n", status.bh_data_ready);
    printf("bh_status_changed: %d\n", status.bh_status_changed);
    printf("uwb_scan_ready: %d\n", status.uwb_scan_ready);
    printf("usr_data_ready: %d\n", status.uwb_scan_ready);
    printf("usr_data_sent: %d\n", status.usr_data_sent);
    printf("fwup_in_progress: %d\n", status.fwup_in_progress);
}

void dump_pos(Position const& pos)
{
    printf("(%d, %d, %d), qf = %d\n", pos.x, pos.y, pos.z, pos.qf);
}

char const *const uwb_mode_str(uint8_t m)
{
    switch (m) {
    case 0:
        return "offline";
    case 1:
        return "passive";
    case 2:
        return "active";
    default:
        return "invalid";
    }
}

void dump_cfg(NodeCfg const& cfg)
{
    printf("mode: %s\n", cfg.mode ? "anchor" : "tag");
    printf("initiator: %s\n", cfg.initiator ? "true" : "false");
    printf("bridge: %s\n", cfg.bridge ? "true" : "false");
    printf("stnry_en: %s\n", cfg.stnry_en ? "true" : "false");
    printf("meas_mode: %s (%d)\n", cfg.meas_mode == 0 ? "TWR" : "unknown", cfg.meas_mode);
    printf("low_power_en: %s\n", cfg.low_power_en ? "true" : "false");
    printf("loc_engine_en: %s\n", cfg.loc_engine_en ? "true" : "false");
    printf("enc_en: %s\n", cfg.enc_en ? "true" : "false");
    printf("led_en: %s\n", cfg.led_en ? "true" : "false");
    printf("ble_en: %s\n", cfg.ble_en ? "true" : "false");
    printf("fw_upd_en: %s\n", cfg.fw_upd_en ? "true" : "false");
    printf("uwb_mode: %s\n", uwb_mode_str(cfg.uwb_mode));
}

void test_read(char const *const portname)
{
    auto dev = SerialDWM1001(portname);

    printf("\nupd_rate_get\n");
    uint16_t ur, urs;
    dev.upd_rate_get(&ur, &urs);
    printf("ur = %u, urs = %u\n", ur, urs);

    printf("\npanid_get\n");
    uint16_t panid;
    dev.panid_get(&panid);
    printf("panid: 0x%04x\n", panid);

    printf("\nnodeid_get\n");
    uint64_t nodeid;
    dev.nodeid_get(&nodeid);
    printf("nodeid: 0x%016lx\n", nodeid);

    printf("\nstatus_get\n");
    Status status;
    dev.status_get(&status);
    dump_status(status);

    printf("\ncfg_get\n");
    NodeCfg cfg;
    dev.cfg_get(&cfg);
    dump_cfg(cfg);
}

void test_gpio()
{
    auto dev = SerialDWM1001("/dev/ttyACM0");

    printf("\ngpio_cfg_output\n"); // LED D12
    dev.gpio_cfg_output(GPIOIdx::GPIO14, true);

    for (int i = 0; i < 4; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        printf("gpio_value_toggle\n");
        dev.gpio_value_toggle(GPIOIdx::GPIO14);
        bool v;
        dev.gpio_value_get(GPIOIdx::GPIO14, &v);
        printf("gpio_value_get: %d\n", v);
    }
}

void test_pos()
{
    auto dev = SerialDWM1001("/dev/ttyACM0");
    Position pos;
    for (int i = 0; i < 10; i++) {
        dev.pos_get(&pos);
        dump_pos(pos);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void test_iot_dl()
{
    auto dev = SerialDWM1001("/dev/ttyACM0");
    while (true) {
        Status status;
        dev.status_get(&status);
        if (status.usr_data_ready) {
            uint8_t dl_data[DWM_USR_DATA_LEN_MAX + 1], dl_data_len;
            dev.usr_data_read(dl_data, &dl_data_len);
            printf("got usr_data: ");
            for (uint8_t i = 0; i < dl_data_len; i++) {
                printf("%02x ", dl_data[i]);
            }
            dl_data[dl_data_len] = 0x00;
            printf("(%s)\n", (char *)dl_data);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main()
{
    test_read("/dev/ttyACM0");
    test_gpio();
    test_pos();
    test_iot_dl();
    return 0;
}
