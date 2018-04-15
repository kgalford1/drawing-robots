#! /bin/bash
# This script lists the available serial connections to the computer and allows you
# to specify which 'x' -> '/dev/cu.usbmodemx' you wish to connect to with default 'x' <- '1432'.
# This script is intended for use with the m3pi robots via USB.
# Requires installation of pySerial -> https://github.com/pyserial/pyserial

echo -e "\nLIST OF SERIAL PORTS"
echo -e "===================="
python -m serial.tools.list_ports
echo ""
read -p "USB Modem number (1432): " NAME
NAME=${NAME:-1432}
echo -e "\nOPENING TERMINAL"
echo -e "===================="
python -m serial.tools.miniterm /dev/cu.usbmodem$NAME