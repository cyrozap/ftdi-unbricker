# ftdi-unbricker

## Usage

- `brick` willl set the PID to zero for every FTDI device with the
  default VID/PID combination that is connected to the computer.
- `unbrick` will set the PID to the default value for every FTDI device
  with PID 0 that is connected to the computer.

## Building

You'll need to build and install version 1.2 of libftdi before compiling this program. 

    gcc brick.c -o brick `pkg-config --cflags --libs libftdi1`
    gcc unbrick.c -o unbrick `pkg-config --cflags --libs libftdi1`
