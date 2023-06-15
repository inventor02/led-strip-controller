# led-strip-controller firmware

This is firmware for an RP2040 microcontroller from Raspberry Pi. We use a Pi Pico soldered directly onto the PCB, but theoretically you could use any other RP2040-based board (or even just an RP2040 itself, if you take care of the voltage).

## Resources

- [https://www.techdesign.be/projects/011/011_waves.htm] is a useful page detailing timing requirements for the Samsung IR protocol - Samsung also have an application note which if you're good at looking you can find, and the timings there match too. In the real world the tolerances are much higher anyway.
