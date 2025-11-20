#ifndef ESP_WAV_PLAYER_WAV_HANDLE_H_
#define ESP_WAV_PLAYER_WAV_HANDLE_H_

#include <stdint.h>
#include <stddef.h>

#include "include/wav_object.h"

typedef struct wav_handle wav_handle_t;

struct wav_handle {
    void *ctx;
    int (*open)(wav_handle_t *h);
    size_t (*read)(wav_handle_t *h, void *buf, size_t len);
    int (*seek)(wav_handle_t *h, size_t offset);
    void (*close)(wav_handle_t *h);
    void (*clean_ctx)(wav_handle_t *h);

    // Filled by wav_parse_header()
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint32_t sample_alignment;
    uint16_t bit_depth;
    size_t   data_start;
    size_t   data_bytes;
};

wav_handle_t *wav_backend_embed_create(const uint8_t *start);
wav_handle_t *wav_backend_file_create(const char *path);

// backend-independent creator
wav_handle_t *wav_handle_init(const wav_obj_t *src);
void          wav_handle_free(wav_handle_t *h);

// parses header and fills h->size, h->sample_rate, etc.
int wav_parse_header(wav_handle_t *h);

#endif /* COMPONENTS_ESP_WAV_PLAYER_WAV_HANDLE_H_ */
