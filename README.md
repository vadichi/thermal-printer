# Thermal Printer

> A driver for the Epson M-T50II thermal printer, easily reconfigurable for different thermal printer models, connecting to their raw GPIO interface, supporting wireless communication.


## Architecture

The printer is connected to an Arduino Uno ("Driver Chip"), which accepts commands through a simple serial-based textual protocol. Optionally, the microcontroller can also be connected to an LCD display and a keypad. The wirless communication is enabled by an ESP8266 ("WiFi Chip"), which can accept commands via a WebSocket over WiFi (in SoftAP mode or connecting to an existing network) and forward them to the Driver Chip.

Several wrappers around the WebSocket connection and the communication protocol are also provided, including a collection of JavaScript helper functions, and a \*nix driver daemon.


## ToDo
- [ ] Add password protection / client authentication
- [ ] Add editable persistent configuration @ Arduino EEPROM
- [ ] Add editable network configuration (connect to existing AP)
    - [ ] Add ping & return MAC functions
- [ ] Add a way to print text (font => lines => dots); on printer or client side?
- [ ] Add a way to print a bitmap (what to do with colours?)
    - [ ] Upload bitmap & break down by lines
    - [ ] Allow smaller bitmaps w/ alignment (R, L, C)
    - [ ] Allow creation from interface
    - [ ] QR codes?
- [ ] Make the UI look nice
- [ ] Allow printing only parts of line in bold (print bold & non-bold sections separately)
    - [ ] Allow printing parts of a bitmap in bold (easy to configure from UI; from bitmap — convert to greyscale; three intervals (0, non-bold, bold)?) 
