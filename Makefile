BUILD_DIR=_build

_animus.hex: animus/*
	arduino-cli -v compile -b arduino:avr:leonardo --libraries libraries --build-path $(BUILD_DIR) animus
	cp $(BUILD_DIR)/animus.ino.hex _animus.hex
