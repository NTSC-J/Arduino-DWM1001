#include <ArduinoSPIDWM1001.h>

ArduinoSPIDWM1001 dev(10);

/*
 * Set up the device as a tag node.
 * These settings will be written to the device's internal flash.
 * You may not need to call this function if there is no change in configuration.
 */
void setup_persistent()
{
  // Configure the node as tag.
  dev.cfg_tag_set({
    .stnry_en = true, // enable stationary detection
    .low_power_en = false, // disable low power mode
    .meas_mode = 0, // measure mode: Two-Way Ranging
    .loc_engine_en = true, // enable location engine
    .enc_en = false, // disable encryption
    .led_en = true, // enable LEDs
    .ble_en = true, // enable Bluetooth Low-Energy
    .uwb_mode = 2, // UWB mode: active
    .fw_upd_en = false // disable firmware update
  });
  
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
  Serial.begin(57600); // console
  Serial.println("init");
  setup_persistent();
}

void loop() {
  Serial.print("panid: ");
  uint16_t panid;
  dev.panid_get(&panid);
  Serial.println(panid, HEX);
  delay(500);
  Serial.print("pos: ");
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
  delay(500);
}
