# STM32 Webserver and Web Frontend Dashboard

This project uses FreeRTOS to run 2 Tasks:

- Mongoose Webserver [main.c](Core/Src/main.c)
- User button listner [TaskBlinker.c](Core/Src/TaskBlinker.c)


## Mongoose Webserver

- This listens for HTTPS connections and validates user sessions before allowing privileged access.
- Users can be a standard user or an admin. 
- An admin can see all details about the board and also upload new firmware. 
- Standard users have very limited access and are read only.

Mongoose is a production-grade embedded web server and networking library for
STM32, NXP, RP2350, ESP32, and other Cortex-M and RISC-V microcontrollers. It
provides HTTP, WebSocket, TLS/HTTPS, MQTT, and OTA firmware updates in a
compact two file C library that runs on bare metal or RTOS, enabling secure
device dashboards, cloud connectivity, and remote management.

## User button listener

- This listens for button presses on startup of the task and memorizes the pattern of the button presses.
- After memorization, it replays the pattern matching the duration of button press and duration of gaps between presses.

## Tools Used

- STM32Cube IDE, MX
- SEGGER J-Link, Ozone, SystemView
- Mongoose Wizard

## Takeaways

This is a very powerful system that can be expanded to connect to MQTT for pub/sub automations for IIoT setups. Modbus is also supported to allow PLC and other dedicated controlers to communicate with the dashboard. Challenges included configuring mongoose to show the user tasks, and setting up the event logging. Future expansions could include querying a database file on an sd card, and appending to the database when users write events.

