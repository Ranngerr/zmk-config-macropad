/* Temporary diagnostic for the user's ZMK v0.3 macropad. */
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(macropad_diag, LOG_LEVEL_INF);

static void macropad_usb_diagnostics(void *a, void *b, void *c)
{
    ARG_UNUSED(a);
    ARG_UNUSED(b);
    ARG_UNUSED(c);

    const struct device *console = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
    uint32_t count = 0;

    while (1) {
        uint32_t dtr = 0;
        int dtr_rc = uart_line_ctrl_get(console, UART_LINE_CTRL_DTR, &dtr);
        int oled_ready = -1;

#if defined(CONFIG_ZMK_DISPLAY) && DT_HAS_CHOSEN(zephyr_display)
        const struct device *oled = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
        oled_ready = device_is_ready(oled) ? 1 : 0;
#endif

        ++count;
        printk("\r\nMACROPAD_DIAG_V1 count=%u uptime_ms=%u console=%s "
               "console_ready=%d DTR=%u DTR_rc=%d OLED_ready=%d\r\n",
               (unsigned int)count, (unsigned int)k_uptime_get_32(),
               console->name, device_is_ready(console) ? 1 : 0,
               (unsigned int)dtr, dtr_rc, oled_ready);
        LOG_INF("MACROPAD_LOG_V1 count=%u", (unsigned int)count);
        k_sleep(K_SECONDS(5));
    }
}

K_THREAD_DEFINE(macropad_usb_diag_thread, 1024, macropad_usb_diagnostics,
                NULL, NULL, NULL, 10, 0, 10000);
