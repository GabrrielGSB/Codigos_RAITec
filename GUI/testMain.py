import csv
import matplotlib.pyplot as plt
import multiprocessing
import numpy as np
import time as t

def sen(x): 
    return np.sin(x)

class EscritorCSV:
    def __init__(self, arquivoCSV='dados.csv'):
        self.arquivo = arquivoCSV
        self.contador = 0
        self.dados = [0, 0]

        # Escreve o cabeçalho no arquivo CSV se ele estiver vazio
        with open(self.arquivo, mode='w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(['Pontos_no_Eixo_X', 'Pontos_no_Eixo_Y'])


    def escrever(self, count):
        while self.contador < 100_000:
            self.dados[0] += 0.1  
            self.dados[1] = sen(self.dados[0])

            with open(self.arquivo, mode='a', newline='') as file:
                writer = csv.writer(file)
                writer.writerow([round(self.dados[0], 3), round(self.dados[1], 3)])
                file.flush()
            t.sleep(0.0001)
            count.value += + 1
            self.contador += 1

class MonitorCSV:
    def __init__(self, file_path):
        self.file_path = file_path
        self.dadosX, self.dadoY = [], []
        self.oldCount = None

    def monitorar(self, count):
        with open(self.file_path, mode='r') as file:
            file.seek(0, 0)  # Move o ponteiro para o início do arquivo

            while True:
                if self.oldCount == count.value:
                    break
                self.oldCount = count.value 
                for row in file:
                    try:
                        # Processa os novos dados
                        dados = row.strip().split(",")  # Lê linha e divide por vírgula
                        self.dadosX.append(float(dados[0]))  # Usando índice ao invés de chaves
                        self.dadoY.append(float(dados[1]))  # Usando índice ao invés de chaves
                        print(float(dados[0]), float(dados[1]))  # Exibe os novos dados

                    except ValueError:
                        break
                # Pausa para eitar loop contínuo
                t.sleep(0.025)  

if __name__ == "__main__":
    csv_file = "dados.csv"

    escritor = EscritorCSV(csv_file)  # Cria o objeto da classe EscritorCSV
    monitor = MonitorCSV(csv_file)  # Cria o objeto da classe MonitorCSV
    shared_var = multiprocessing.Value('i', 0)  # 'i' significa tipo inteiro
    p1 = multiprocessing.Process(target=escritor.escrever, args=(shared_var,))
    p2 = multiprocessing.Process(target=monitor.monitorar, args=(shared_var,))

    # Inicia os processos
    p1.start()
    p2.start()

    # Espera os processos terminarem
    p1.join()
    p2.join()
