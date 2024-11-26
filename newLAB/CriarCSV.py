import csv
import serial
from datetime import datetime
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import time

# Gerar um nome de arquivo único com base na data e hora
timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
csv_file = f"dados_{timestamp}.csv"

# Configuração da porta serial
ser = serial.Serial('COM6', 115200)

# Cria o novo arquivo e escreve o cabeçalho
with open(csv_file, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['angle_roll', 'angle_pitch', 'Tempo'])

# Inicializa listas para armazenar os dados
times = []
angle_rolls = []

# Função para atualizar o gráfico em tempo real
def update_plot(frame):
    global times, angle_rolls
    
    with open(csv_file, mode='r') as file:
        reader = csv.DictReader(file)
        times = []
        angle_rolls = []

        # Lê todos os dados do arquivo
        for row in reader:
            try:
                times.append(float(row['Tempo']))
                angle_rolls.append(float(row['angle_roll']))
            except ValueError:
                continue

    # Limpa e atualiza o gráfico
    ax.clear()
    ax.plot(times, angle_rolls, label='Angle Roll', color='blue')
    ax.set_title("Angle Roll vs Time")
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Angle Roll (degrees)")
    ax.legend()
    ax.grid(True)

# Configuração do Matplotlib
fig, ax = plt.subplots()
ani = FuncAnimation(fig, update_plot, interval=500)  # Atualiza a cada 1 segundo

# Processo de escrita no arquivo CSV
def write_to_csv():
    try:
        while ser.is_open:
            line = ser.readline().decode('utf-8').split(',')
            print("Recebido da Serial:", line)  # Mostra a linha recebida

            try:
                # Extração de valores
                angleRoll = line[0].split(':')[1].strip()
                anglePitch = line[1].split(':')[1].strip()
                tempo = line[2].split(':')[1].strip()

                # Escreve os dados no CSV
                with open(csv_file, mode='a', newline='') as file:
                    writer = csv.writer(file)
                    writer.writerow([angleRoll, anglePitch, tempo])
                    file.flush()  # Garante que os dados sejam gravados imediatamente

            except Exception as e:
                print("Erro ao processar linha:", e)

    except KeyboardInterrupt:
        print("Finalizando...")
    finally:
        ser.close()

# Executa a gravação no CSV em um processo separado
import threading
thread = threading.Thread(target=write_to_csv, daemon=True)
thread.start()

# Mostra o gráfico em tempo real
plt.show()
