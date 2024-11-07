import serial
import time
import csv

# Inicializa a variável de controle para o CSV
CSVcriado      = False
equipeRecebida = False
tempoRecebido  = False


ser = serial.Serial('/dev/ttyUSB0', 115200) # Configura a porta serial (substitua 'COM6' pela sua porta)
csv_filename = 'dados_equipes.csv'          # Arquivo CSV para salvar os dados

# Função para enviar dados serial
def enviar_dados_serial(dado):
    if dado.strip():                       # Verifica se o dado não é apenas uma linha em branco
        ser.write((dado + '\n').encode())  # Adiciona uma nova linha e envia o dado

# Função para salvar dados no arquivo CSV
def salvar_dados_csv(equipe, tempo):
    global CSVcriado  # Declara que estamos usando a variável global
    try:
        if not CSVcriado:
            with open(csv_filename, mode='w', newline='') as file:  # Modo de criação
                writer = csv.writer(file)
                writer.writerow(['Equipe', 'Tempo']) # Escreve o cabeçalho do CSV
                writer.writerow([ equipe ,  tempo])  # Escreve os dados de equipe e tempo na mesma linha
            print(f"Salvo no CSV: Equipe={equipe}, Tempo={tempo}")
            CSVcriado = True
        else:
            with open(csv_filename, mode='a', newline='') as file:  # Modo de adição
                writer = csv.writer(file)
                writer.writerow([equipe, tempo])  # Escreve os dados de equipe e tempo na mesma linha
            print(f"Salvo no CSV: Equipe={equipe}, Tempo={tempo}")
    except Exception as e:
        print(f"Erro ao salvar no CSV: {e}")

try:
    while True:
        # Captura a string do usuário apenas se a equipe não estiver definida
        if equipeRecebida == False:
            print("")
            dado_usuario = input("Cadastrar Equipe: ").strip()
        
        if dado_usuario:  # Verifica se o input não está vazio
            print("enviei")
            enviar_dados_serial(dado_usuario)
            print(f"Enviado: {dado_usuario}")

            if equipeRecebida == False: equipe = ""
            if tempoRecebido  == False: tempo  = ""

            # start_time = time.time()
            # timeout = 3  # Tempo máximo de espera em segundos

            # Recebe dados (se houver)
            while ((equipeRecebida and tempoRecebido) == False):
                if ser.in_waiting > 0:
                    # print(equipeRecebida)
                    # print(tempoRecebido)
                    # print("")
                    line = ser.readline().decode('utf-8').strip()
                    print("Recebido:", line)

                    # Verifica se a linha recebida é "Equipe cadastrada:"
                    if line.startswith("Equipe cadastrada:"):
                        equipeRecebida = True
                        equipe         = line.split(":")[1].strip()  # Extrai o nome da equipe

                    # Verifica se a linha recebida é "Tempo:"
                    if line.startswith("Tempo:"):
                        tempoRecebido = True
                        tempo         = line.split(":")[1].strip()  # Extrai o tempo
            
            # Se ambos 'equipe' e 'tempo' estiverem preenchidos, salvar no CSV
            if equipe and tempo:
                print("to quasd")
                salvar_dados_csv(equipe, tempo)
                tempoRecebido  = False
                equipeRecebida = False

except KeyboardInterrupt:
    print("Interrupção pelo usuário")
finally:
    ser.close()  # Fecha a conexão serial ao terminar