import serial

print("Abrindo portas!")

ser = serial.Serial('/dev/ttyUSB2', 115200)
port1 = serial.Serial('/dev/tnt0', 115200)
port2 = serial.Serial('/dev/tnt1', 115200)

if (port1.is_open and port2.is_open):
    print('Portas abertas com succeso!')

while ser.is_open:
    try:
        line = ser.readline()
        port1.write(line)
        port2.write(line)
        line = line.decode('utf-8')
        print(line)
    except:
        pass

ser.close()
port1.close()
port2.close()
