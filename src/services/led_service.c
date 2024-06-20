#include "led_service.h"

#define BT_UUID_LED_SERVICE BT_UUID_DECLARE_128(LED_SERVICE_UUID)
#define BT_UUID_LED_CHARACTERISTIC BT_UUID_DECLARE_128(LED_CHARACTERISTIC_UUID)

static write_callback_t write_callback;
static read_callback_t read_callback;

void led_service_init(write_callback_t write_cb, read_callback_t read_cb)
{
    write_callback = write_cb;
    read_callback = read_cb;
}

static ssize_t on_write(struct bt_conn *conn,
                        const struct bt_gatt_attr *attr,
                        const void *buf,
                        uint16_t len,
                        uint16_t offset,
                        uint8_t flags)
{
    if (write_callback != NULL)
    {
        write_callback(buf, len);
    }
}

static ssize_t on_read(struct bt_conn *conn,
                       const struct bt_gatt_attr *attr,
                       void *buf,
                       uint16_t len,
                       uint16_t offset)
{
    uint8_t *encoded_buffer = read_callback();
    return bt_gatt_attr_read(conn, attr, buf, len, offset,
                             encoded_buffer, sizeof(encoded_buffer));
}

BT_GATT_SERVICE_DEFINE(led_service,
                       BT_GATT_PRIMARY_SERVICE(BT_UUID_LED_SERVICE),
                       BT_GATT_CHARACTERISTIC(BT_UUID_LED_CHARACTERISTIC,
                                              BT_GATT_CHRC_WRITE | BT_GATT_CHRC_READ,
                                              BT_GATT_PERM_WRITE | BT_GATT_PERM_READ,
                                              on_read, on_write, NULL), );