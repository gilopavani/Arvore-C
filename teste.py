import os
import time
from random import randint
import numpy
tempo = {}
teste = 100 #define a quantidade de testes
for x in range(teste):
    arquivo = open('in.txt', 'w') #
    for i in range(1000): #quantidade de valores inseridos
        arquivo.write(str(randint(1,1000)))
        arquivo.write(' ')
    arquivo.write('0')
    arquivo.close()

        

    ini = time.time()
    os.system('gcc -o exe ./main.c')
    os.system('exe < in.txt')
    fim = time.time()
    tempo[x] = fim - ini
    

print(teste)
soma = 0.0
for x in range(teste):
    soma = soma + tempo[x]


print(soma/teste)