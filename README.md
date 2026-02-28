# Collect-Data-Over-Network
This simple project provides a base for assembling a quick and reproducible battery powered device to read sensor measurements and log their variations over a period of time.

This is done by having an ESP board read values from sensors and then connecting to a FaaS endpoint to effectively store those values. Another FaaS endpoint is then provided to retrieve those values in numeric form or a graph form. The ESP board then enters a power-saving state (deep sleep) between measurements to save up on battery.

At the current state the project is made with the following specification:
- Using the ESP8266 board
- Measuring luminosity, temperature and humidity using a photoresistor for the former and a DHT11 module for the two latter
- Using Supabase Cloud Service as the service to handle the database (PostgreSQL) and FaaS (Edge Functions)
- Even with this set specifications, it should be easy to add new modules/sensors or change the behaviors of the logging


## Requirements and deployment for the current state
### Hardware needed
- ESP8266 development board
- Breadboard and wire jumpers, or another mean of connecting the components such as a perfboard
- DHT11 (or equivalent) for reading temperature and humidity
- Photoresistor

### Cloud Services needed (for running)
- Supabase Account for their cloud services
	- It can be self hosted too and should work equivalently
	- The free tier for the cloud hosted account is sufficient but be aware there is a limit on the amount of projects and that the projects will be paused after a week of inactivity 

### Software needed (for compiling and Flashing)
- Arduino IDE
- ESP8266 compatibility for Arduino IDE (see deployment instructions)
- DHT11 library (see deployment instructions)

### Deployment instructions
Download the Source Code and follow the instructions for the hardware side and the remote side of the project.

Remote deployment:
1. Login into your Supabase Account, create (or enter) an organization and a project
2. Enter the SQL editor and run the commands to create the tables
	1. Equivalently, you can create the tables manually
3. Enter the Edge Functions section and create the necessary functions
4. Copy the logging edge function endpoint

Hardware Deployment:
1. Wire the components according to the schematic
2. Open the Arduino IDE and connect the ESP8266 to the computer
	1. If you are using linux, you might need to add your user to the `uucp` or `dialout` group 
2. Make sure to have ESP8266 compatibility installed on the Arduino IDE and select the connected board as the target board with the correct board type
	1. If you don't see ESP8266 as a board type, you likely do not have the compatibility installed
	2. Include "https://arduino.esp8266.com/stable/package_esp8266com_index.json" as an additional board manager provider in the Arduino IDE preferences
	3. Install "ESP8266" from "ESP8266 Community" from the Arduino IDE board manager
	4. Select "Generic ESP8266 Module" as the board type
	5. Ensure everything was correctly set up by flashing an example blinking sketch onto the ESP8266
3. Open the sketch, including the config file
	1. An example config file exists as "config.h.example", copy it without the ".example" suffix and include this one
	2. Do set up the network SSID and password in the included config
4. Make sure all libraries are installed correctly from the Arduino IDE's library manager
	1. Install ArduinoJson and DHT11 (from "dhrubasaha08")
5. Flash the sketch

Once you have followed the instructions, you should be seeing the tables periodically getting filled with data.
