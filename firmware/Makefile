
CFLAGS = -Wall -Wextra -pedantic -O2
CFLAGS += -DF_CPU=1000000
CFLAGS += -g

MCU = attiny44a
# for avrdude
aMCU = t44a

SRC=$(wildcard *.c)

all: main.hex

main.hex: main.elf
	avr-objcopy -j .data -j .text -O ihex main.elf main.hex

main.elf: $(SRC)
	avr-gcc -mmcu=$(MCU) -o $@ $^ $(CFLAGS)

flash: main.hex
	avrdude -p $(aMCU) -c atmelice_isp -e -U flash:w:main.hex

clean:
	rm -f *.elf *.hex *.o
