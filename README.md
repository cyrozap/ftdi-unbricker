# ftdi-unbricker

[![Build Status](https://travis-ci.org/cyrozap/ftdi-unbricker.svg?branch=master)](https://travis-ci.org/cyrozap/ftdi-unbricker)
[![GPLv2 License](http://img.shields.io/badge/license-GPLv2-blue.svg)](https://www.gnu.org/licenses/gpl-2.0.html)

## Usage

- `brick` willl set the PID to zero for every FTDI device with the
  default VID/PID combination that is connected to the computer.
- `unbrick` will set the PID to the default value for every FTDI device
  with PID 0 that is connected to the computer.

## Building

You'll need to build and install version 1.2 of libftdi before
compiling this program. Once you have it installed, simply run `make`
and everything should compile.
