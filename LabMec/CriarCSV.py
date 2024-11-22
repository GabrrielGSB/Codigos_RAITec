import csv
import serial

def criarCSV_Serial(nomeArquivo="dados.csv"):
    ser = serial.Serial('/dev/ttyUSB0', 115200)
    with open(nomeArquivo, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['angle_roll','angle_pitch','Tempo'])

        while ser.is_open:
            line = ser.readline().decode('utf-8').split(',')

            try:
                angleRoll  = line[0].split(':')[1].strip()
                anglePitch = line[1].split(':')[1].strip()
                tempo      = line[2].split(':')[1].strip()

                writer.writerow([angleRoll, anglePitch, tempo])
                
            except:
                pass
                      
    ser.close()
