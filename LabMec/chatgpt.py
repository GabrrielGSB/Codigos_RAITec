from multiprocessing import Process
import time

# Função 1: Executa um loop infinito com prints
def funcao1():
    while True:
        print("Função 1 está rodando...")
        time.sleep(1)  # Pausa de 1 segundo para evitar sobrecarga

# Função 2: Executa um loop infinito com prints
def funcao2():
    while True:
        print("Função 2 está rodando...")
        time.sleep(1)  # Pausa de 1 segundo para evitar sobrecarga

if __name__ == "__main__":
    # Criando os processos
    processo1 = Process(target=funcao1)
    processo2 = Process(target=funcao2)

    # Iniciando os processos
    processo1.start()
    processo2.start()

    # Mantendo o programa principal ativo
    try:
        processo1.join()
        processo2.join()
    except KeyboardInterrupt:
        print("\nEncerrando os processos...")
        processo1.terminate()
        processo2.terminate()
