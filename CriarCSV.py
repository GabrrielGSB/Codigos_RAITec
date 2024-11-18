import serial
import csv

ser = serial.Serial('COM4', 115200)

with open('dados.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['angle_roll','angle_pitch','Kalman_Angle_Roll','Kalman_Angle_Pitch','Tempo'])

    while ser.is_open:
        line = ser.readline()
        print(line)
        try:
            line = line.decode('utf-8')
            values = line.split(',')

            angle_roll         = values[0].split(':')[1]
            angle_pitch        = values[1].split(':')[1]
            kalman_angle_roll  = values[2].split(':')[1]
            kalman_angle_pitch = values[3].split(':')[1]
            tempo              = values[4].split(':')[1]
            # print(kalman_angle_roll)

            writer.writerow([angle_roll,angle_pitch,kalman_angle_roll, kalman_angle_pitch, tempo])
                
        except:
            pass

ser.close()
