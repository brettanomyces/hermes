Hermes
======

# PlatformIO

http://docs.platformio.org/en/latest/quickstart.html

## Build project

    platformio run

## Upload firmware

    platformio run --target upload

## Monitor serial conneciton

    platformio serialports monitor


# Serial connection

## Add user to dialout group

    sudo adduser <username> dialout

## View settings

    stty --file /dev/<tty> --all
    
## Enable auto reset on serial connection (this is the default)

    stty --file /dev/<tty> hup

## Disable auto reset on serial connection

    stty --file /dev/<tty> -hup
