VERSION = 0.01
#FAMILY = mega
MCU_NAME = 328
TARGET		= bassboycc
PACKAGES  = avrlib bassboycc

F_CPU = 20000000
PROGRAMMER_PORT = usb
PROGRAMMER = arduinoisp

AVRDUDE_ERASE = no
AVRDUDE_LOCK = no

## Nano
#PROGRAMMER_PORT = /dev/tty.usbserial-A6030UOD
#PROGRAMMER = arduino
#F_CPU = 16000000


EXTRA_DEFINES = -DNDEBUG

include avrlib/makefile.mk

include $(DEP_FILE)
