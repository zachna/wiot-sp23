/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <sys/printk.h>
#include <sys/util.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>


static void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type,
                    struct net_buf_simple *buf)
{
    char src_addr[18];

    // Convert address to typical MAC address format.
    bt_addr_le_to_str(addr, src_addr, 18);

    // Print data only if RSSI is above -70 dBm
    if (rssi > -30) {
        printk("[BLE ADV] src: %s (rssi: %i)\n", src_addr, rssi);
    }
	else if(rssi > -70){
		printk("[BLE ADV] src: %s (rssi: %i)\n", src_addr, rssi);
	}
}

void main(void)
{
    struct bt_le_scan_param scan_param = {
        .type       = BT_HCI_LE_SCAN_PASSIVE,
        .options    = BT_LE_SCAN_OPT_NONE,
        .interval   = 0x0010,
        .window     = 0x0010,
    };
    int err;

    printk("Starting Scanner\n");

    // Initialize the Bluetooth Subsystem
    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth init failed (err %d)\n", err);
        return;
    }

    printk("Bluetooth initialized\n");

    // Enable active scanning (Scan Requests)
    scan_param.type = BT_HCI_LE_SCAN_ACTIVE;

    err = bt_le_scan_start(&scan_param, scan_cb);
    if (err) {
        printk("Starting scanning failed (err %d)\n", err);
        return;
    }

    printk("Scanning started\n");

    // Scan for 10 seconds and count the total number of advertisements received
    k_sleep(K_SECONDS(10));
    int count = bt_le_scan_stop();
    printk("Scanning stopped (advertisements received: %d)\n", count);
}