# FT107m# FT107m
This repository contain source code to arduino hardware that control DMS unit from FT-107m to 
set and get frequency as same time read S-meter data like signal, voltage, tx power and so on. 

A brief overview how to control a FT107m through of a computer. 
https://www.youtube.com/watch?v=GOnllGdeZFk

More information can be found at readme2.

The project have three steps, the first are related to basic control of DMS unit to setup frequency , to read frequency from digital display and to present S-Meter signal value. 
The second, is related with the replace DMS by a VFO DDS and clone all functionality of it. Finally, the third will let total control of the radio



As there are problems with the reliability of the
Frequency counter that uses a custom IC to 
Count the frequency, add / substract an offset
programmed by a diode matrix and to drive a
multiplexed LED Display best would be to design
an fpga/cpld to mimic this function and to 
send the frequency count to the micro controller 
using I2C, SPI or UART.
Interesstingly the mode (USB, LSB, CW can be
seen from the data lines to the counter device 
for the frequency shift to be calculated by pre
setting the counter register, this information can
be sent to the interface also) 

About the pain of the dead MSM9520RS yaesu special
device (maybe, an early cpld / PAL that lost its data)
MSM9520RS ist most likely a relabled PAL as the number
of hamradio transceivers was not high enough to 
justify the cost of asic design / full custom chip.
A sea of gates device could also be a possibility.
https://web.archive.org/web/20210417140653/https://w

The pain of the MSM9520RS 
ww.qsl.net/oh8jep/ja2svz/

https://www.qsl.net/oh8jep/ja2svz/
