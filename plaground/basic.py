import serial
import serial.tools.list_ports as ports_list
import time

# List available ports with indices
ports = ports_list.comports()
serial_instance = serial.Serial()
ports_List = []

print("Available ports:")
for index, port in enumerate(ports):
    ports_List.append(str(port))
    print(f"{index}: {port}")

# Ask user to select port by index
try:
    index = int(input("Select port index: "))
    use = ports_List[index].split(' - ')[0]  # Extract the port path
    print(f"Using {use}")
except (ValueError, IndexError):
    print("Invalid index selected")
    exit()

serial_instance.baudrate = 9600
serial_instance.port = "/dev/cu.usbmodem11101"

try:
    serial_instance.open()
except serial.SerialException as e:
    print(f"Could not open port {use}: {e}")
    exit()

for i in range(10):
    time.sleep(2)
    command = "Javier"
    serial_instance.write(command.encode("utf-8"))

if command.lower() == "exit":
    serial_instance.close()
    exit()

	


# arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1) 


# def write_read(x): 
# 	arduino.write(bytes(x, 'utf-8')) 
# 	time.sleep(0.05) 
# 	data = arduino.readline() 
# 	return data 

# while True: 
# 	num = input("Enter a number: ") # Taking input from user 
# 	value = write_read(num) 
# 	print(value) # printing the value 
