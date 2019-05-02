# rpi-kt0803l

Configure the KT0803L FM radios over i2c on the Raspberry Pi.

## Build and Install

Before building, you will need to install wiringpi

    sudo apt-get update && sudo apt-get install wiringpi

Then run make:

    make
    sudo make install

You should now be able to run the `rpi-kt0803l` command.

## Usage

See the internal help page for usage information:

    rpi-kt0803l --help
