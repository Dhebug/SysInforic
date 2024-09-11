# SysInforic
Work in progress tool to show the specs of an oric machine

Right now all it does is:
- Show the amount of memory (16K or 48K)
- Print a CRC32 value of the top 16K of memory and matches that with a list of known configurations

The CRC32 code is from http://www.6502.org/source/integers/crc.htm
