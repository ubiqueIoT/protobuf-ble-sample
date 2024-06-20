#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>

#include "services/led_service.h"
#include "lib/bluetooth.h"
#include "lib/proto_helper.h"

#define RED_LED_NODE DT_ALIAS(led0)
#define GREEN_LED_NODE DT_ALIAS(led1)
#define BLUE_LED_NODE DT_ALIAS(led2)

static const struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(RED_LED_NODE, gpios);
static const struct gpio_dt_spec green_led = GPIO_DT_SPEC_GET(GREEN_LED_NODE, gpios);
static const struct gpio_dt_spec blue_led = GPIO_DT_SPEC_GET(BLUE_LED_NODE, gpios);

static led_color_t led_color = {
	.red = 0,
	.green = 1,
	.blue = 0,
};

static uint8_t encoded_buffer[LEDColorMessage_size];
static size_t message_length;

static int leds_init(void)
{
	if (!device_is_ready(red_led.port) ||
		!device_is_ready(green_led.port) ||
		!device_is_ready(blue_led.port))
	{
		printk("One or more LEDs are not ready\n");
		return -ENODEV;
	}
	return 0;
}

static void update_leds()
{
	int red_state = led_color.red == 1 ? GPIO_OUTPUT_ACTIVE : GPIO_OUTPUT_INACTIVE;
	int green_state = led_color.green == 1 ? GPIO_OUTPUT_ACTIVE : GPIO_OUTPUT_INACTIVE;
	int blue_state = led_color.blue == 1 ? GPIO_OUTPUT_ACTIVE : GPIO_OUTPUT_INACTIVE;

	gpio_pin_configure_dt(&red_led, red_state);
	gpio_pin_configure_dt(&green_led, green_state);
	gpio_pin_configure_dt(&blue_led, blue_state);
}

void write_callback(uint8_t *data, uint16_t len)
{
	led_color = decode_message(data, len);
	update_leds();
}

uint8_t *read_callback()
{
	encode_message(encoded_buffer, sizeof(encoded_buffer), &message_length, &led_color);

	printk("Encoded message length: %d\n", LEDColorMessage_size);
	for (int i = 0; i < LEDColorMessage_size; i++)
	{
		printk("%d ", encoded_buffer[i]);
	}
	printk("\n");
	return encoded_buffer;
}

int main(void)
{
	ble_init();
	led_service_init(write_callback, read_callback);
	leds_init();
	update_leds();

	while (1)
	{
		k_sleep(K_FOREVER);
	}
}