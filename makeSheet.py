import serial
import csv

ser = serial.Serial('/dev/tnt1', 115200)

with open('dados.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['Motor1', 'Motor2', 'Motor3', 'Motor4', 'Angle_Roll', 'Angle_Pitch', 'Kalman_Angle_Roll', 'Kalman_Angle_Pitch', 'Tempo'])

    while ser.is_open:
        line = ser.readline()
        
        try:
            line = line.decode('utf-8')
            if "Motor1:" in line:
                # Divide a linha recebida por vírgula para separar os valores
                values = line.split(',')
                    
                # Extrai os valores de cada motor
                motor1 = values[0].split(':')[1]
                motor2 = values[1].split(':')[1]
                motor3 = values[2].split(':')[1]
                motor4 = values[3].split(':')[1]

                angle_roll = values[4].split(':')[1]
                angle_pitch = values[5].split(':')[1]

                kalman_angle_roll = values[6].split(':')[1]
                kalman_angle_pitch = values[7].split(':')[1]

                tempo = values[8].split(':')[1]


                writer.writerow([motor1, motor2, motor3, motor4, angle_roll, angle_pitch, kalman_angle_roll, kalman_angle_pitch, tempo])
                
        except:
            pass

ser.close()
