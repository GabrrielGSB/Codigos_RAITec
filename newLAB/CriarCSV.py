import csv
import serial
from datetime import datetime
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import multiprocessing

def plot_animation(csv_file):
    # Inicializa listas para armazenar os dados
    times = []
    angle_rolls = []

    # Função para atualizar o gráfico em tempo real
    def update_plot(frame):
        nonlocal times, angle_rolls

        with open(csv_file, mode='r') as file:
            reader = csv.DictReader(file)
            times = []
            angle_rolls = []

            # Lê todos os dados do arquivo
            for row in reader:
                try:
                    # Convertendo o tempo para segundos
                    time_in_seconds = float(row['Tempo']) / 1000  # Se estiver em milissegundos
                    times.append(time_in_seconds)
                    angle_rolls.append(float(row['angle_roll']))
                except ValueError:
                    continue

        # Limpa e atualiza o gráfico
        ax.clear()
        ax.plot(times, angle_rolls, label='Angle Roll', color='blue')
        ax.set_title("Angle Roll vs Time")
        ax.set_xlabel("Time (s)")  # Unidades em segundos
        ax.set_ylabel("Angle Roll (degrees)")
        ax.legend()
        ax.grid(True)

    # Configuração do Matplotlib
    fig, ax = plt.subplots()
    ani = FuncAnimation(fig, update_plot, interval=250)  # Atualiza a cada 250 ms
    plt.show()


def write_to_csv(csv_file):
    ser = serial.Serial('COM6', 115200)  # Configuração da porta serial

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


if __name__ == "__main__":
    # Gerar um nome de arquivo único com base na data e hora
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    csv_file = f"dados_{timestamp}.csv"

    # Cria o novo arquivo e escreve o cabeçalho
    with open(csv_file, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['angle_roll', 'angle_pitch', 'Tempo'])

    # Cria processos
    p1 = multiprocessing.Process(target=plot_animation, args=(csv_file,))
    p2 = multiprocessing.Process(target=write_to_csv, args=(csv_file,))

    # Inicia processos
    p1.start()
    p2.start()

    # Espera os processos terminarem
    p1.join()
    p2.join()
