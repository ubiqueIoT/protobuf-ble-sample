#include "bluetooth.h"

struct bt_conn *connection;
static K_SEM_DEFINE(ble_init_ok, 0, 1);

static void connected(struct bt_conn *conn, uint8_t err)
{
    struct bt_conn_info info;
    char addr[BT_ADDR_LE_STR_LEN];

    connection = conn;

    if (err)
    {
        printk("Connection failed (err %u)\n", err);
        return;
    }
    else if (bt_conn_get_info(conn, &info))
    {
        printk("Could not parse connection info\n");
    }
    else
    {
        bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
        printk("Connection established!		\n\
		Connected to: %s					\n\
		Role: %u							\n\
		Connection interval: %u				\n\
		Slave latency: %u					\n\
		Connection supervisory timeout: %u	\n",
               addr, info.role, info.le.interval, info.le.latency, info.le.timeout);
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    printk("Disconnected: %d\n", reason);
    connection = NULL;
}

static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
    .le_param_req = NULL,
    .le_param_updated = NULL};

static void bt_ready(int err)
{
    if (err)
    {
        printk("BLE init failed with error code %d\n", err);
        return;
    }

    bt_conn_cb_register(&conn_callbacks);

    if (err)
    {
        printk("Failed to init services (err:%d)\n", err);
        return;
    }

    struct bt_data ad[] = {
        BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
        BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME)),
    };

    struct bt_data sd[] = {
        BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME)),
    };

    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad),
                          sd, ARRAY_SIZE(sd));

    if (err)
    {
        printk("Advertising failed to start (err %d)\n", err);
        return;
    }
    k_sem_give(&ble_init_ok);
}

void ble_init()
{
    int err = 0;

    err = bt_enable(bt_ready);
    if (err)
    {
        printk("BLE initialization failed\n");
    }

    err = k_sem_take(&ble_init_ok, K_MSEC(500));

    if (err)
    {
        printk("BLE initialization did not complete in time\n");
    }
}
