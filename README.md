# SmartGardenESP32
# ESP32 OTA and Web Server Project

This project demonstrates how to set up an ESP32 with Wi-Fi connectivity, OTA (Over-The-Air) updates, and a basic HTTP server. The ESP32 is configured to handle OTA firmware updates, serve data from sensors, and provide a web interface for querying sensor data and system health.

## Features

- **Wi-Fi Connectivity**: Connects to a Wi-Fi network.
- **OTA Updates**: Allows firmware updates over the air with optional password protection.
- **HTTP Server**: Provides endpoints to access sensor data and system status.
- **DHT22 Sensor**: Reads temperature and humidity.
- **Soil Moisture Sensor**: Reads soil moisture levels.

## Requirements

- **Hardware**:
  - ESP32 board
  - DHT22 temperature and humidity sensor
  - Soil moisture sensor

- **Software**:
  - [Arduino IDE](https://www.arduino.cc/en/software)
  - [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)

## Installation

1. **Install the ESP32 Board in Arduino IDE**:
   - Open the Arduino IDE.
   - Go to **File > Preferences** and add the following URL to the "Additional Boards Manager URLs" field:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to **Tools > Board > Boards Manager**, search for `esp32`, and install the latest version.

2. **Install Required Libraries**:
   - Go to **Sketch > Include Library > Manage Libraries**.
   - Install the following libraries:
     - `ESPAsyncWebServer`
     - `DHT sensor library`
     - `ArduinoJson`
     - `ESPmDNS`

3. **Clone or Download the Project**:
   - Clone this repository or download the code to your local machine.

4. **Configure Wi-Fi Settings**:
   - Open the file `wifi_config.h` and replace `your_SSID` and `your_WIFI_PASSWORD` with your Wi-Fi network credentials.

5. **Upload the Code**:
   - Connect your ESP32 to your computer via USB.
   - Select the correct board and port in the Arduino IDE.
   - Upload the code to the ESP32.

## Usage

1. **Connect to Wi-Fi**:
   - The ESP32 will connect to the Wi-Fi network specified in `wifi_config.h`.

2. **Access the Web Server**:
   - Once connected, the ESP32 will print its IP address to the Serial Monitor.
   - Open a web browser and enter the IP address to access the web server.

3. **OTA Updates**:
   - To update the firmware over the air, use an OTA client and provide the configured password if required.

4. **HTTP Endpoints**:
   - **/healthcheck**: Returns the system health status in JSON format.
     ```json
     {"status":"healthy"}
     ```
   - **/guid**: Returns a hardcoded GUID.
     ```json
     {"guid":"123e4567-e89b-12d3-a456-426614174000"}
     ```
   - **/temperature-humidity**: Returns the current temperature and humidity readings from the DHT22 sensor.
     ```json
     {"temperature":22.5,"humidity":60.0}
     ```
   - **/soil-moisture**: Returns the soil moisture level.
     ```json
     {"soilMoisture":512}
     ```

## Security

- **OTA Password**: The OTA password is set in the `setupOTA()` function. Make sure to use a secure password to prevent unauthorized updates.

## Troubleshooting

- **Device Not Connecting to Wi-Fi**: Verify that your Wi-Fi credentials in `wifi_config.h` are correct and ensure the network is operational.
- **OTA Update Fails**: Check the Serial Monitor for errors related to OTA. Make sure you are using the correct password.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- ESP32 Arduino Core: [ESP32 Arduino](https://github.com/espressif/arduino-esp32)
- ESPAsyncWebServer: [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- DHT Sensor Library: [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)
- ArduinoJson: [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

For further assistance, please refer to the [ESP32 documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/) or check the [ESP32 community forums](https://www.esp32.com/).
