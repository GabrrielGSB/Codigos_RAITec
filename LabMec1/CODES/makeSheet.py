import serial
import csv

ser = serial.Serial('/dev/tnt1', 115200)

with open('dados.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['Angle_Roll', 'Angle_Pitch', 'Tempo'])

    while ser.is_open:
        line = ser.readline()
        
        try:
            line = line.decode('utf-8')
            if "Motor1:" in line:
                # Divide a linha recebida por vírgula para separar os valores
                values = line.split(',')
                    
                # Extrai os valores de cada motor
                angle_roll = values[0].split(':')[1]
                angle_pitch = values[1].split(':')[1]
                tempo = values[3].split(':')[1]


                writer.writerow([angle_roll, angle_pitch, tempo])
                
        except:
            pass

ser.close()
