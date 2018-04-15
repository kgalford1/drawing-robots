#! /bin/bash
# This script lists the available serial connections to the computer and allows you
# to specify which 'x' -> '/dev/ttyx' you wish to connect to with default 'x' <- 'USB0'.
# Requires installation of pySerial -> https://github.com/pyserial/pyserial

echo -e "\nLIST OF SERIAL PORTS"
echo -e "===================="
python -m serial.tools.list_ports
echo ""
read -p "USB Modem number (USB0): " NAME
NAME=${NAME:-USB0}
echo -e "\nOPENING TERMINAL"
echo -e "===================="
python -m serial.tools.miniterm /dev/tty$NAME
