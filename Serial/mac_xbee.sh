#! /bin/bash
# This script lists the available serial connections to the computer and allows you
# to specify which 'x' -> '/dev/cu.usbserial-x' you wish to connect to with default 'x' <- 'AH02V9G9'.
# This script is intended for use with XBee USB breakout boards.
# Requires installation of pySerial -> https://github.com/pyserial/pyserial

echo -e "\nLIST OF SERIAL PORTS"
echo -e "===================="
python -m serial.tools.list_ports
echo ""
read -p "USB Serial (AH02V9G9): " NAME
NAME=${NAME:-AH02V9G9}
echo -e "\nOPENING TERMINAL"
echo -e "===================="
python -m serial.tools.miniterm /dev/cu.usbserial-$NAME