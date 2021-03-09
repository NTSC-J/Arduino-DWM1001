/*
 * Read downlink usr_data sent from bridge to tag.
 */

#include <ArduinoSPIDWM1001.h>

ArduinoSPIDWM1001 dev(10);
uint64_t nodeid;

void print_nodeid() {
  Serial.print((uint32_t)(nodeid >> 32), HEX);
  Serial.println((uint32_t)(nodeid & 0xffffffff), HEX);
}

void setup() {
  Serial.begin(57600);
  dev.nodeid_get(&nodeid);
}

void loop() {
  Status status;
  dev.status_get(&status);
  if (status.usr_data_ready) {
    Serial.println("usr_data_ready");
    uint8_t data[DWM_USR_DATA_LEN_MAX + 1], len;
    dev.usr_data_read(data, &len);
    Serial.print("usr_data: ");
    for (uint8_t i = 0; i < len; i++) {
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  } else {
    Serial.println("usr_data not ready, nodeid: ");
    print_nodeid();
  }
  delay(200);
}
