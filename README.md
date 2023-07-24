# Arduino-Printer

A driver for the raw interface of the Epson M-T50II thermal printer based on an Arduino Uno with wireless communication based on an ESP8266.

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
