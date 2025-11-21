# esp_wav_player
Simple wav player component for ESP-IDF / ESP8266_RTOS_SDK

esp_wav_player is a lightweight component for ESP32 and ESP8266 that plays PCM WAV files from local storage or streamed sources. It supports common PCM WAV formats (8-bit and 16-bit), mono and stereo channels, and typical sample rates (8 kHz–48 kHz). The component provides a simple API to initialize the player/I2S, start/stop playback, and feed WAV data from SPIFFS, SD card (FAT), or network streams.

Features

- Supports PCM WAV (8-bit and 16-bit), mono and stereo
- Plays files from SPIFFS/FAT or from streams
- Simple playback API: initialize, play, pause, stop
- Works with ESP-IDF and ESP8266_RTOS_SDK
- Example project included in the examples/ directory

Basic usage

1. Add esp_wav_player as a component in your project (components/esp_wav_player).
2. Initialize I2S and the wav player with your configuration.
3. Start playback from a file or stream (e.g., wav_player_play_file("/spiffs/example.wav")).
4. Stop or pause playback as needed.

See examples/default/README.md for example pin mappings and a quickstart for ESP32/ESP8266.
