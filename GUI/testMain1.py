import socket

# Configurações do cliente
HOST = '127.0.0.1'  # Endereço IP do servidor (localhost)
PORT = 65432        # Porta do servidor

# Cria o socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
    client_socket.connect((HOST, PORT))  # Conecta ao servidor
    client_socket.sendall('Olá, servidor!')  # Envia dados
    data = client_socket.recv(1024)  # Recebe a resposta do servidor

print(f"Recebido do servidor: {data.decode('utf-8')}")
