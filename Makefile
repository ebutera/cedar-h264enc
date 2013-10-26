CC = arm-angstrom-linux-gnueabi-gcc
CPP = arm-angstrom-linux-gnueabi-g++
STRIP = arm-angstrom-linux-gnueabi-strip
AR = arm-angstrom-linux-gnueabi-ar

CFLAGS += -Wall -DOS_LINUX -I./cedarv
LIBS += -pthread ./lib/libcedarv.a ./lib/libh264enc.a ./lib/libcedarv_osal.a ./lib/libcedarxalloc.a -lv4lconvert

SRC := $(wildcard *.c)
OBJ := $(SRC:%.c=%.o)

TARGET := EncoderDemo
.PHONY : clean all

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CPP) $(CFLAGS) -o $@ $^ $(LIBS)
	$(STRIP) $@

clean:
	@rm -f $(TARGET)
	@rm -f $(OBJ)

