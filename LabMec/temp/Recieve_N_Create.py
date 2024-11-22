import socket
import csv

# Configurações do servidor
host = "0.0.0.0"
port = 5000

# Lista para armazenar os dados recebidos
data_list = []

# Abrir o arquivo CSV para escrita
with open('dados.csv', mode='w', newline='') as file:
    writer = csv.writer(file)

    # Escrever o cabeçalho no CSV
    writer.writerow(['RateRoll', 'RatePitch', 'RateYaw'])

    # Configura o servidor de sockets
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.bind((host, port))
        server_socket.listen()
        server_socket.settimeout(10)  # Timeout de 10 segundos

        print("Aguardando conexão do ESP32...")

        # Loop para aceitar conexões
        while True:
            try:
                # Aceitar conexão do cliente (ESP32)
                client_socket, addr = server_socket.accept()
                print(f"Conectado a {addr}")

                # Loop para receber dados do cliente
                with client_socket:
                    while True:
                        data = client_socket.recv(1024).decode('utf-8')

                        if not data:
                            # Se não houver mais dados, o cliente desconectou
                            print("Cliente desconectado.")
                            break  # Sai do loop de recepção de dados

                        print(f"Dado recebido: {data}")

                        # Extrair os valores RateRoll, RatePitch e RateYaw
                        try:
                            values = data.split(": ")[1].split()  #Extrai os valores necessários.
                            RateRoll = float(values[0])
                            RatePitch = float(values[1])
                            RateYaw = float(values[2])

                            # Escrever os dados no CSV
                            writer.writerow([RateRoll, RatePitch, RateYaw])

                        except Exception as e:
                            print(f"Erro ao processar os dados: {e}")

            except socket.timeout:
                print("Esperando por uma nova conexão...")
