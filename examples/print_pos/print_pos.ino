#include <ArduinoSPIDWM1001.h>

constexpr uint8_t PIN_CS = 1;
constexpr uint8_t PIN_DRDY = 2;
ArduinoSPIDWM1001 dev(PIN_CS, PIN_DRDY); // DRDY is optional

/*
 * Set up the device as a tag node.
 * These settings will be written to the device's internal flash.
 * You may not need to call this function if there is no change in configuration.
 */
void setup_persistent()
{
  // Configure the node as tag.
  TagCfg cfg = {
    .stnry_en = true,
    .low_power_en = false,
    .meas_mode = 0, // TWR
    .loc_engine_en = true,
    .enc_en = false,
    .led_en = true,
    .ble_en = true,
    .uwb_mode = 2, // active
    .fw_upd_en = false
  };
  dev.cfg_tag_set(cfg);
  
  // How often the tag updates its location (times 100 ms)
  // 1st argument: while tag is moving
  // 2nd argument: while tag is stationary
  dev.upd_rate_set(1, 10);

  // How sensitive the accelerometer should be.
  dev.stnry_cfg_set(StnrySensitivity::SS_NORMAL);

  // The network PANID.
  dev.panid_set(0xb428);
}

void setup() {
  Serial.begin(115200); // console
  setup_persistent();
}

void loop() {
  Position pos;
  dev.pos_get(&pos);
  Serial.print("(");
  Serial.print(pos.x);
  Serial.print(", ");
  Serial.print(pos.y);
  Serial.print(", ");
  Serial.print(pos.z);
  Serial.print("), qf = ");
  Serial.print(pos.qf);
  Serial.println();
}
