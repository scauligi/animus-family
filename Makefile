BUILD_DIR=_build

_animus.hex: animus/*
	arduino-cli -v compile -b arduino:avr:leonardo --libraries libraries --build-path $(BUILD_DIR) animus
	cp $(BUILD_DIR)/animus.ino.hex _animus.hex

all: _animus.hex _dump_eeprom.hex

upload: _animus.hex
	avrdude -cavr109 -patmega32u4 -P/dev/tty.usbmodemCHIDKH1 -b57600 -D -v -Uflash:w:_animus.hex:i

_dump_eeprom.hex: dump_eeprom/*
	arduino-cli -v compile -b arduino:avr:leonardo --libraries libraries --build-path $(BUILD_DIR) dump_eeprom
	cp $(BUILD_DIR)/dump_eeprom.ino.hex _dump_eeprom.hex

upload_dump: _dump_eeprom.hex
	avrdude -cavr109 -patmega32u4 -P/dev/tty.usbmodemCHIDKH1 -b57600 -D -v -Uflash:w:_dump_eeprom.hex:i

