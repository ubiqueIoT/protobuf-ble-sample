#include "proto_helper.h"

static led_color_t led_color = {
    .red = 0,
    .green = 0,
    .blue = 0,
};

bool encode_message(uint8_t *buffer, size_t buffer_size, size_t *message_length, led_color_t *led_color)
{
    LEDColorMessage message = LEDColorMessage_init_zero;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_size);

    message.red = led_color->red;
    message.green = led_color->green;
    message.blue = led_color->blue;

    bool status = pb_encode(&stream, LEDColorMessage_fields, &message);
    *message_length = stream.bytes_written;

    if (!status)
    {
        printk("Encoding failed: %s\n", PB_GET_ERROR(&stream));
    }

    return status;
}

led_color_t decode_message(uint8_t *buffer, size_t message_length)
{
    LEDColorMessage message = LEDColorMessage_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);
    bool status = pb_decode(&stream, LEDColorMessage_fields, &message);

    if (status)
    {
        printk("New LED set to: %d %d %d\n", (int)message.red, (int)message.green, (int)message.blue);
        led_color.red = (int)message.red;
        led_color.green = (int)message.green;
        led_color.blue = (int)message.blue;
    }
    else
    {
        printk("Decoding failed: %s\n", PB_GET_ERROR(&stream));
    }

    return led_color;
}
