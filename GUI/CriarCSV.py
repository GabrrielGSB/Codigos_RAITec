import csv
import serial
from datetime import datetime
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import multiprocessing
import numpy as np
import time as t
def sen(x): return np.sin(x)

def criarGraficoDinamico(csv_file):
    # Inicializa listas para armazenar os dados
    dadosX, dadosY = [], []

    # Função para atualizar o gráfico em tempo real
    def atualizarGrafico(frame):
        nonlocal dadosX, dadosY

        # Lê apenas a última linha do arquivo
        with open(csv_file, mode='r') as file:
            try:
                last_line = list(csv.DictReader(file))[-1]  # Obtém a última linha
                ultimo_x = float(last_line['Pontos_no_Eixo_X'])
                ultimo_y = float(last_line['Pontos_no_Eixo_Y'])

                # Adiciona o último ponto aos dados
                dadosX.append(ultimo_x)
                dadosY.append(ultimo_y)

                # Limpa e atualiza o gráfico
                ax.clear()
                ax.plot(dadosX, dadosY, label='dados', color='blue')
                ax.set_title("Angle Roll vs Time")
                ax.set_xlabel("Time (s)")  # Unidades em segundos
                ax.set_ylabel("Angle Roll (degrees)")
                ax.legend()
                ax.grid(True)
            except (ValueError, IndexError, KeyError):
                # Ignora erros se o arquivo estiver vazio ou com dados inválidos
                pass

    # Configuração do Matplotlib
    fig, ax = plt.subplots()
    ani = FuncAnimation(fig, atualizarGrafico, interval=25)
    plt.show()

def escreverCSV(csv_file):
    ser = serial.Serial('COM6', 115200)  # Configuração da porta serial

    try:
        while ser.is_open:
            line = ser.readline().decode('utf-8').split(',')
            print("Recebido da Serial:", line)  # Mostra a linha recebida

            try:
                # Extração de valores
                angleRoll  = line[0].split(':')[1].strip()
                anglePitch = line[1].split(':')[1].strip()
                tempo      = line[2].split(':')[1].strip()

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

def escreverCSVTeste(arquivoCSV='dados.csv'):
    contador = 0
    dados = [0,0]

    while contador < 400:
        dados[0] += 0.1  
        dados[1] = sen(dados[0])
       
        with open(arquivoCSV, mode='a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow([round(dados[0],3), round(dados[1],3)])
            file.flush()  
        t.sleep(0.1)
        contador += 1

if __name__ == "__main__":
    # Gerar um nome de arquivo único com base na data e hora
    timestamp = datetime.now().strftime("%Y_%m_%d__%H_%M_%S")
    csv_file = f"dados_{timestamp}.csv"

    # Cria o novo arquivo e escreve o cabeçalho
    with open(csv_file, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['Pontos_no_Eixo_X', 'Pontos_no_Eixo_Y'])

    # Cria processos
    p1 = multiprocessing.Process(target=criarGraficoDinamico, args=(csv_file,))
    p2 = multiprocessing.Process(target=escreverCSVTeste,     args=(csv_file,))

    # Inicia processos
    p1.start()
    p2.start()

    # Espera os processos terminarem
    p1.join()
    p2.join()