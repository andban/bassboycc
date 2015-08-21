
include bassboycc/Makefile

FIRMWARE = $(BUILD_DIR)bassboycc.hex
BOOTLOADER = $(BUILD_ROOT)muboot/muboot.hex

upload_all:	$(FIRMWARE) $(BOOTLOADER)
		$(AVRDUDE) $(AVRDUDE_COM_OPTS) $(AVRDUDE_ISP_OPTS) \
				-U flash:w:$(FIRMWARE):i -U flash:w:$(BOOTLOADER):i

#bake_all: $(FIRMWARE) $(BOOTLOADER)
#		make -f bootloaded/makefile fuses
#		$(AVRDUDE) -B 1 $(AVRDUDE_COM_OPTS) $(AVRDUDE_ISP_OPTS) \
#			-U flash:w:$(FIRMWARE):i -U flash:w:$(BOOTLOADER):i \
#			-U lock:w:0x2f:m
