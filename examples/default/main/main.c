/* Simple WAV playback example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_system.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event_loop.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <driver/gpio.h>
#include <driver/i2s.h>

#include <esp_wav_player.h>

static const char *TAG = "APP";

static xQueueHandle gpio_evt_queue = NULL;

extern const uint8_t _binary_darude_wav_start[];

WAV_DECLARE_EMBED(wav_example, _binary_darude_wav_start);

void play_start_cb(esp_wav_player_t player, void *arg)
{
    ESP_LOGI(TAG, "WAV player start callback");
}

void play_end_cb(esp_wav_player_t player, void *arg)
{
    ESP_LOGI(TAG, "WAV player stop callback");
}

static void gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

void setup_gpio_interrupt(void)
{
    gpio_config_t io_conf;

    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.pin_bit_mask = (1 << GPIO_NUM_0);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_NUM_0, gpio_isr_handler, (void *)GPIO_NUM_0);
}

void app_main()
{
    esp_wav_player_config_t player_conf = ESP_WAV_PLAYER_DEFAULT_CONFIG();
    esp_wav_player_t        wav_player;
    esp_wav_player_state_t  st;
    uint32_t                io_num;

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_LOGI(TAG, "WAV player demo. Press GPIO0 to play WAV");
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_wav_player_init(&wav_player, &player_conf);
    esp_wav_player_set_volume(wav_player, 50);
    esp_wav_player_set_start_cb(wav_player, play_start_cb, NULL);
    esp_wav_player_set_end_cb(wav_player, play_end_cb, NULL);
    setup_gpio_interrupt();
    for (;;) {
        if (xQueueReceive(gpio_evt_queue, &io_num, 10)) {
            esp_wav_player_get_state(wav_player, &st);
            switch (st) {
            case ESP_WAV_PLAYER_STOPPED:
                ESP_LOGI(TAG, "wav player start");
                esp_wav_player_play(wav_player, &wav_example);
                break;
            case ESP_WAV_PLAYER_PLAYING:
            case ESP_WAV_PLAYER_PAUSED:
                esp_wav_player_pause(wav_player);
                break;
            default:
                break;
            }
            size_t free_heap = esp_get_free_heap_size();
            ESP_LOGI("HEAP", "Free heap: %d bytes", free_heap);
        }
        vTaskDelay(250 / portTICK_RATE_MS);
    }
}
