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

    cat /dev/<tty>
