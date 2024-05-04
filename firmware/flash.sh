avr-objcopy -j .data -j .text -O ihex build/Debug/discolights discolights.hex
avrdude -c atmelice_isp -p t44a -U flash:w:discolights.hex
