import openPort as op
import CriarCSV as crv
import multiprocessing as Process

if __name__ == "__main__":
    
    # Criando processos
    processo2 = Process(target = crv.criarCSV_Serial())
    processo1 = Process(target = op.open_port())
    
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
