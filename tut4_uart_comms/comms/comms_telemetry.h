#ifndef TUT4_COMMS_TELEMETRY_H
#define TUT4_COMMS_TELEMETRY_H

#include <stdint.h>

#define TELEMETRY_UID (uint16_t)0x2A

typedef struct __attribute__((packed)) {
    uint8_t uid;
    uint32_t cookie;
    uint8_t switch_on;
} telemetry_t;

void comms_update_telemetry(telemetry_t new_telemetry);

#endif
