Hermes
======

Install the latest Arduino IDE

# Build

    arduino --verify hermes.ino

# Upload To Arduino

    arduino --upload hermes.ino

# Enable auto reset on serial connection

    stty -F /dev/<tty> -hupcl
    
# Enable auto reset on serial connection

    stty -F /dev/<tty> hupcl
    
# Add user to dialout group - allows you to view output without needing sudo 

    sudo adduser <username> dialout
    
# View serial output
`moreutils` must be installed for `ts`, `sudo apt-get install moreutils`. $TZ must also be set to get the correct time `tzselect`

    cat /dev/<tty> | ts >> hermes.log &
    tail -f hermes.log
    
    
# Bill of Materials

* Arduino UNO R3 (pretty unreliable)
* 2 X 2 Relay Arduino modules
* L298N Stepper Motor Driver
* Stepper motor - voltage/current?
* Thermistors?
* Resistors for voltage divider to measure temp
* Transformer
