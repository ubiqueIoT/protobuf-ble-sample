#ifndef LED_SERVICE_H
#define LED_SERVICE_H

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/kernel.h>
#include <soc.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/hci.h>

#include "../lib/user_types.h"

#define LED_SERVICE_UUID BT_UUID_128_ENCODE(0xEC0C6AB1, 0x66C9, 0x4A2C, 0x917A, 0x8B5879D9FCAA)

#define LED_CHARACTERISTIC_UUID BT_UUID_128_ENCODE(0xEC0C6AB1, 0x66C9, 0x4A2C, 0x917A, 0x8B5879D9FCAB)

typedef void (*write_callback_t)(uint8_t *data, uint16_t len);
typedef uint8_t *(*read_callback_t)();

void led_service_init(write_callback_t write_cb, read_callback_t read_cb);

#endif // LED_SERVICE_H