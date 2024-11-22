import matplotlib.pyplot as plt
import matplotlib.animation as animation
from openPort import port1

fig = plt.figure()
ax  = fig.add_subplot()

def animate(i, x_vals, y_vals, ser):
    line = port1.readline().decode('utf-8') 
    
    #print(line)

    try:
        if "Motor1:" in line:
            # Divide a linha recebida por vírgula para separar os valores
            values = line.split(',')
                
            # Extrai os valores de cada motor
            angle_roll  = float(values[0].split(':')[1])
            angle_pitch = float(values[1].split(':')[1])
            tempo       = float(values[2].split(':')[1])

            x_vals.append(tempo)
            y_vals.append(angle_roll)
    except:
        pass

    
    ax.clear()
    x_vals = x_vals[-50:]
    y_vals = y_vals[-50:]
    ax.plot(x_vals,y_vals)

    



