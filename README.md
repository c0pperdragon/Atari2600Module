# Atari2600Module
Game module board for Atari 2600 that can hold 16 games.

After I have obtained and repaired an old Atari 2600 console (a "light sixer" PAL variant), 
I also wanted to use it with the top games of the time. Working game cartridges can still be found
on action platforms, but it is very difficult and expensive to get the famous ones. 
On the other hand, basically everything that was ever released for the Atari 2600 is available 
as rom images on the web (PAL variants included).

Since the electronics of those days was pretty easy to handle for hobbyist, making an own 
cartridge with an EPROM (can still be obtained in PID variants) and some small electronics parts
is possible for little money.

To keep it as cheap as possible, I have not created a PCB (don't have the equipment), but
used a standard board with two layers of copper lines and cut it to the proper shape with a jigsaw.
According to the sources on the topic the Atari 2600 uses a not-inverted signal to access the 
chip select line of the EPROM, so an inverter is also on the board.
The EPROM I found was a 64KByte variant, so it could hold 16 of the standard-sized 4 KByte 
game ROMs. Selecting is done with an 16-position encoder switch that directly drives the 
higher 4 address lines of the EPROM.

One of the greatest problems in this project was to burn the EPROM. It seems that EPROMs have
completely gone out of use this days and the programmers that can still be bought are extremely
expensive. For this one-off programming I have used my Arduino Micro with one additional 8-bit 
shift register to do the job. I improvised everything on breadboards for the task. Unfortunately 
I have no schematics or images of this left, only the programmer code is still available. 

