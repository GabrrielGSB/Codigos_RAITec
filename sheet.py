import csv
import matplotlib
import matplotlib.pyplot as mat
import numpy as np

motor1 = []
motor2 = []
motor3 = []
motor4 = []
angle_rool = []
angle_pitch = []
kalman_rool = []
kalman_pitch = []
tempo = []

#Importante tirar a a linha que contem somente os nomes, acho que buga
with open('dados.csv', 'r') as arquivo:
    leitor_csv = csv.reader(arquivo)
    
    for linha in leitor_csv:
        motor1.append(int(linha[0]))
        motor2.append(int(linha[1]))
        motor3.append(int(linha[2]))
        motor4.append(int(linha[3]))
        angle_rool.append(float(linha[4]))
        angle_pitch.append(float(linha[5]))
        kalman_rool.append(float(linha[6]))
        kalman_pitch.append(float(linha[7]))
        tempo.append(float(linha[7]))
        

       
#Coloque aki qual dado você quer analisar :)
dado = kalman_pitch

dado = np.array(dado)
tempo = np.array(tempo)


#mat.plot(dado)  # se quiser só os pontos

fig, grafico = mat.subplots(figsize=(9, 9))

grafico = mat.scatter(tempo, dado, s = 60, alpha=0.7, edgecolors="k")

# deg=1 means linear fit (i.e. polynomial of degree 1)
# Ai não entendi direito isso, acho q é pq ele só vê do primeiro grau...

mat.show()
