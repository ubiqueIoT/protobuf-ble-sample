#ifndef PROTO_HELPER_H
#define PROTO_HELPER_H

#include <pb_encode.h>
#include <pb_decode.h>
#include "src/proto/led_color.pb.h"
#include "user_types.h"

bool encode_message(uint8_t *buffer, size_t buffer_size, size_t *message_length, led_color_t *led_color);
led_color_t decode_message(uint8_t *buffer, size_t message_length);

#endif // PROTO_HELPER_H