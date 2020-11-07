# Alarm Player

This project demonstrates the implementation of an alarm player in C++.

It allows the creation of custom alarms, with assignable priority levels.

Alarm playback is mocked-up in a terminal by printing 'X' and '\_' representing the emiision of a beep (X) or a silence (\_)

## Compilation

The project uses CMake, follow the step bellow to compile it.
- Clone or download the repository to a folder on your PC
- Open a terminal to the root of this folder and enter the following commands:
    - `mkdir build && cd build`
    - `cmake ..`
    - `make`

## Run tests

From the build directory created during the compilation, you can run the following commands depending on desired level of details:
- Summary of test result: `make test`
- Complete test report: `./bin/unit_tests`

> Test framework used is [Google Test](https://code.google.com/p/googletest)

## Run Sample CLI

A sample Command Line Interface is available to test the alarm system.

From the build directory created during the compilation, run the following command to launch the sample CLI:
- `./bin/alarm_player`

```
Sample CLI interface for Alarm player

         Press 'h' followed by ENTER to toggle High-level alarm
         Press 'm' followed by ENTER to toggle Medium-level alarm
         Press 'l' followed by ENTER to toggle Low-level alarm
         Press 'q' followed by ENTER to exit the program
```
