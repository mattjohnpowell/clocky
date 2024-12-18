# Clocky - A Kid-Friendly Sleep Training Clock

A fun ESP32-based sleep training clock that helps children know when it's time to wake up or go to bed using friendly Spider-Man animations and color changes. Built for the Guition JC8048W550C_I ESP32-S3 5-inch IPS display board.

## Features

- Digital clock display with date
- Customizable wake-up and bedtime settings
- Friendly Spider-Man animations for bedtime notifications
- Dynamic background colors (bright for day, dark for night)
- Easy-to-use touch interface for time adjustments
- WiFi-enabled for automatic time synchronization
- Clean, modern UI with smooth transitions

## Hardware Requirements

- Guition JC8048W550C_I ESP32-S3 5-inch IPS display board (16M FLASH, 8M PSRAM)
- USB-C cable for programming
- 5V power supply

## Software Requirements

- PlatformIO IDE
- Visual Studio Code
- ESP32 Arduino framework
- LVGL library (included)

## Installation

1. Clone the repository with submodules:
   ```bash
   git clone --recurse-submodules https://github.com/[your-username]/clocky.git
   ```

2. Copy `config.h.local` to `config.h` and update with your settings:
   ```cpp
   #define WIFI_SSID "your_wifi_ssid"
   #define WIFI_PASSWORD "your_wifi_password"
   #define SELECTED_TIMEZONE "your_timezone"  // e.g., "EST5EDT,M3.2.0,M11.1.0"
   ```

3. Open the project in PlatformIO IDE (Visual Studio Code)

4. Install required board definition:
   - Copy the board file `boards/esp32-8048S050C.json` to your PlatformIO boards directory
   - Or add the boards repository to your `platformio.ini`:
     ```ini
     [env:esp32-8048S050C]
     platform = espressif32
     board = esp32-8048S050C
     framework = arduino
     ```

5. Build and upload the project to your device

## Usage

1. Power on the device - it will automatically connect to WiFi and sync time
2. Use the + and - buttons to adjust wake-up and sleep times
3. The display will show:
   - Current time in large digits
   - Current date
   - Wake-up time setting
   - Bedtime setting
   - Spider-Man animations near bedtime
   - Day/night appropriate background colors

## Customization

The clock face and animations can be customized by modifying:
- UI layout in `ui_scrMain.c`
- Background colors in `style_bg_day` and `style_bg_night`
- Animation timing in `BEDTIME_WINDOW_HOURS`
- Spider-Man images (replace `spiderman.c` and `spidermansleep.c`)

## Troubleshooting

- If WiFi doesn't connect, check your credentials in `config.h`
- If time doesn't sync, verify your timezone setting
- For display issues, ensure correct board selection in PlatformIO
- USB CDC may require a 5-second delay on some systems

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

[Your chosen license]

## Acknowledgments

- Based on the esp32-smartdisplay library
- Uses LVGL for the user interface
- Spider-Man images and references are property of their respective owners