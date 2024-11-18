import serial.tools.list_ports as list_ports

all_ports = list_ports.comports()

for port in all_ports:
    print(f'Porta: {port.device} | Descrição: {port.description}')
