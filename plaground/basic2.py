import serial
import serial.tools.list_ports as ports_list
import time

# List available ports with indices
ports = ports_list.comports()
serial_instance = serial.Serial()

use = "/dev/cu.usbmodem11101"
serial_instance.baudrate = 9600
serial_instance.port = use


for i in range(10):
    time.sleep(2)
    command = "Javier"
    serial_instance.write(command.encode("utf-8"))

if command.lower() == "exit":
    serial_instance.close()
    exit()