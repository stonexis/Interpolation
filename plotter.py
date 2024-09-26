import json
import matplotlib.pyplot as plt

def plot_graph(filename):
    #Чтение JSON данных из файла
    with open(filename, 'r') as file:
        data = json.load(file)

    arrX = data['arrX']
    arrFx = data['arrFx']
    arrLagrange = data['arrLagrange']
    arrNodesX = data['arrNodesX']
    valueInterpolation = data['valueInterpolation']

    plt.figure(figsize=(15, 8))
    plt.minorticks_on()
    plt.grid(which='major')
    plt.grid(which='minor')
    plt.plot(arrX, arrFx, color='blue', linewidth=2, label='fun')
    plt.plot(arrX, arrLagrange, 'r--', linewidth=1.5, label='Lagrange')
    plt.plot(arrNodesX, valueInterpolation,'gs', markersize=4) #выделение точек интерполяции
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.legend()
    plt.show()