import time
import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation


def animate(i, x_vals, y_vals, ser):
    line = ser.readline().decode('utf-8') 
    #print(line)

    try:
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
            tempo = int(tempo)

            dado = kalman_angle_roll
            dado = float(dado)

            x_vals.append(tempo)
            y_vals.append(dado)
    except:
        pass

    
    ax.clear()
    x_vals = x_vals[-50:]
    y_vals = y_vals[-50:]
    ax.plot(x_vals,y_vals)

    

x_vals = []
y_vals = []

fig = plt.figure()
ax = fig.add_subplot()


ser = serial.Serial('/dev/tnt0', 115200)


ani = animation.FuncAnimation(fig, animate, frames=100, fargs=(x_vals, y_vals, ser), interval=100)


plt.show()
ser.close()
