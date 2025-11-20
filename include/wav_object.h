#ifndef _WAV_OBJECT_H_
#define _WAV_OBJECT_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WAV_SRC_EMBED, // WAV file embedded in program memory
    WAV_SRC_FILE,  // WAV file in SPIFFS filesystem
    WAV_SRC_MMC,   // WAV file on MMC/SD card
} wav_source_type_t;

typedef struct {
    wav_source_type_t type;
    union {
        struct {
            const uint8_t *addr; // pointer to embedded data
        } embed;
        struct {
            const char *path; // path in SPIFFS
        } file;
        struct {
            const char *path; // path on MMC
        } mmc;
    };
} wav_obj_t;

// Macros to declare WAV objects
#define WAV_DECLARE_EMBED(name, addr) \
    static const wav_obj_t name = { .type = WAV_SRC_EMBED, .embed = { addr } }

#define WAV_DECLARE_FILE(name, path) \
    static const wav_obj_t name = { .type = WAV_SRC_FILE, .file = { path } }

#define WAV_DECLARE_MMC(name, path) \
    static const wav_obj_t name = { .type = WAV_SRC_MMC, .mmc = { path } }

#ifdef __cplusplus
}
#endif

#endif // _WAV_OBJECT_H_
