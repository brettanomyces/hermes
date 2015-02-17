Hermes
======

# Build

    ino clean
    ino build

# Upload To Arduino

    ino upload

# View Serial Output

    cat /dev/ttyXXX > log/hermes.log
    tail -f log/hermes.log

# Send commands

    // Set fridge temp
    echo '2, 20.0;' > /dev/ttyXXX

    // set freezer temp
    echo '3, 10.0;' > /dev/ttyXXX
