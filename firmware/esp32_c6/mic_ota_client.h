#ifndef MIC_OTA_CLIENT_H
#define MIC_OTA_CLIENT_H
#include <stddef.h>
#include <stdint.h>
/* ESP32-C6 portable OTA client: supply actual half-duplex 1Wire TX/RX
 * callbacks; image A/B must be separately linked for their addresses. */
typedef struct {
 int (*send)(void *ctx,const uint8_t *bytes,size_t n);
 int (*recv)(void *ctx,uint8_t *bytes,size_t n,uint32_t timeout_ms);
 void *ctx;
} mic_ota_bus_t;
typedef enum {MIC_OTA_OK=0,MIC_OTA_IO=-1,MIC_OTA_PROTOCOL=-2,
 MIC_OTA_IMAGE=-3,MIC_OTA_RETRY_EXHAUSTED=-4} mic_ota_result_t;
mic_ota_result_t mic_ota_upload(const mic_ota_bus_t *bus,
 const uint8_t *image_a,size_t size_a,const uint8_t *image_b,size_t size_b,
 uint8_t *target_slot);
#endif
