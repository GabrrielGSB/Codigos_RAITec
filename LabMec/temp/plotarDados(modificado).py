import csv
import matplotlib.pyplot as plot

#angle_roll = []
#angle_pitch = []
#kalman_roll = []
#kalman_pitch = []
#tempo = []
listas = []

with open('Dados.csv', 'r') as arquivo:
    leitor_csv = csv.reader(arquivo)

    # Ignora a linha de cabeçalho, se necessário
    next(leitor_csv, None)

    for linha in leitor_csv:
        try:
            listas.append(float(linha[0]))
        except ValueError:
            print(f"Erro ao converter linha: {linha}")

# Dados para o gráfico
dado1 = listas
tempo = range(len(listas))  # Cria um intervalo baseado no número de pontos
# Criando o gráfico
plot.figure()
plot.plot(tempo, dado1)  # Colocando 'tempo' no eixo X e 'dado1' no eixo Y
plot.ylim(-5, 90)
plot.title("Gráfico 1")
plot.xlabel("Tempo")
plot.ylabel("Valores")
plot.show()