#pragma once

#ifdef ARDUINO
#include <stdint.h>
#else
#include <cstdint>
#endif

class DWM1001TLV {
public:
    /*
    * @brief TLV types for commands
    */
    enum CMD : uint8_t {
        POS_SET          = 0x01,  /* command set position coordinates XYZ */
        POS_GET          = 0x02,  /* command get position coordinates XYZ */
        UR_SET           = 0x03,  /* command set position update rate*/
        UR_GET           = 0x04,  /* command get position update rate*/
        CFG_TN_SET       = 0x05,  /* command set configuration for the tag */
        CFG_AN_SET       = 0x07,  /* command set configuration for the anchor */
        CFG_GET          = 0x08,  /* command get configuration data */
        SLEEP            = 0x0a,  /* command sleep */
        AN_LIST_GET		 = 0x0b,  /* command anchor list get */
        LOC_GET          = 0x0c,  /* command location get */
        BLE_ADDR_SET     = 0x0f,  /* command BLE address set */
        BLE_ADDR_GET     = 0x10,  /* command BLE address get */
        STNRY_CFG_SET    = 0x11,  /* command stationary configuration set */
        STNRY_CFG_GET    = 0x12,  /* command stationary configuration get */
        FAC_RESET        = 0x13,  /* command factory reset */
        RESET            = 0x14,  /* command reset */
        VER_GET          = 0x15,  /* command FW version get */
        UWB_CFG_SET		 = 0x17,  /* command UWB configuration set */
        UWB_CFG_GET		 = 0x18,  /* command UWB configuration get */
        USR_DATA_READ    = 0x19,  /* command user data read */
        USR_DATA_WRITE   = 0x1a,  /* command user data write */
        LABEL_READ       = 0x1c,  /* command lebel read */
        LABEL_WRITE      = 0x1d,  /* command label write */
        UWB_PREAMBLE_SET = 0x1e,  /* command set uwb preamble code */
        UWB_PREAMBLE_GET = 0x1f,  /* command get uwb preamble code */
        UWB_SCAN_START   = 0x23,  /* command UWB scan start */
        UWB_SCAN_RES_GET = 0x24,  /* command UWB scan results get */
        GPIO_CFG_OUTPUT  = 0x28,  /* command configure output pin and set */
        GPIO_CFG_INPUT   = 0x29,  /* command configure input pin */
        GPIO_VAL_SET     = 0x2a,  /* command set pin value */
        GPIO_VAL_GET     = 0x2b,  /* command get pin value */
        GPIO_VAL_TOGGLE  = 0x2c,  /* command toggle pin value */
        PANID_SET        = 0x2e,  /* command panid set */
        PANID_GET        = 0x2f,  /* command panid get */
        NODE_ID_GET      = 0x30,  /* command node id get */
        STATUS_GET       = 0x32,  /* command status get */
        INT_CFG_SET      = 0x34,  /* command configure interrupts */
        INT_CFG_GET      = 0x35,  /* command get interrupt configuration */
        BACKHAUL_XFER    = 0x37,  /* command BACKHAUL data transfer */
        BH_STATUS_GET    = 0x3a,  /* command to get UWBMAC status*/
        ENC_KEY_SET      = 0x3c,  /* command to set security key */
        ENC_KEY_CLEAR    = 0x3d,  /* command to set security key */
        VA_ARG_POS_SET   = 0x80,  /* VA ARG command set position */
    //   N_LOC_GET             = 130,   /* nested request location get */
    };

    /*
    * @brief TLV types for return values
    */
    enum TLV : uint8_t {
        RET_VAL            = 0x40,  /* request return value (as the response) */
        POS_XYZ            = 0x41,  /* position coordinates x,y,z*/
        POS_X              = 0x42,  /* position coordinate x */
        POS_Y              = 0x43,  /* position coordinate y */
        POS_Z              = 0x44,  /* position coordinate z */
        UR                 = 0x45,  /* update rate */
        CFG                = 0x46,  /* configuration data */
        INT_CFG            = 0x47,  /* interrupt configuration */
        RNG_AN_DIST        = 0x48,  /* ranging anchor distances*/
        RNG_AN_POS_DIST    = 0x49,  /* ranging anchor distances and positions*/
        STNRY_SENSITIVITY  = 0x4a,  /* stationary sensitivity */
        USR_DATA           = 0x4b,  /* user data */
        LABEL              = 0x4c,  /* label */
        PANID              = 0x4d,  /* PANID */
        NODE_ID            = 0x4e,  /* node ID */
        UWB_CFG			   = 0x4f,  /* UWB configuration */
        FW_VER             = 0x50,  /* fw_version */
        CFG_VER            = 0x51,  /* cfg_version */
        HW_VER             = 0x52,  /* hw_version */
        PIN_VAL            = 0x55,  /* pin value */
        AN_LIST            = 0x56,  /* anchor list */
        STATUS             = 0x5a,  /* status */
        UWB_SCAN_RESULT    = 0x5c,  /* UWB scan result */
        UWBMAC_STATUS      = 0x5d,  /* UWBMAC status */
        BLE_ADDR           = 0x5f,  /* BLE address */
        DOWNLINK_CHUNK_0   = 0x64,  /* downlink data chunk nr.1 */
        BUF_IDX            = 0x69,  /* index of the buffer in the container */
        BUF_SIZE           = 0x6a,  /* data size in the buffer of the container */
        UPLINK_CHUNK_0     = 0x6e,  /* uplink data chunk nr.1 */
        UPLINK_LOC_DATA    = 0x78,  /* FIXME: set proper define name and value */
        UPLINK_IOT_DATA    = 0x79,  /* FIXME: set proper define name and value */
        VAR_LEN_PARAM      = 0x7f,  /* parameter with variable length */
        IDLE               = 0xff,
    };
};
