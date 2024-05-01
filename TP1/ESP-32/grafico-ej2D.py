import numpy as np
import matplotlib.pyplot as plt

# datos recogidos ejercicio 2D
tiempos = {
    1000: [1, 0.8, 1.2, 1.1, 0.9],
    10000: [10, 9.5, 10.5, 10.2, 9.8],
    100000: [213, 210, 215, 211, 212],
    1000000: [2000, 1995, 2005, 1998, 2001]
}

buffers = []
promedios = []
desviaciones = []

for size, times in tiempos.items():
    buffers.append(size)
    promedios.append(np.mean(times))
    desviaciones.append(np.std(times))

# gráfico de barras
plt.bar(range(len(promedios)), promedios, yerr=desviaciones, tick_label=[str(b) for b in buffers], color='blue', alpha=0.7)
plt.xlabel('Tamaño del Buffer (bytes)')
plt.ylabel('Tiempo de Transmisión (ms)')
plt.title('Promedio y Desviación Estándar de los Tiempos de Transmisión')
plt.show()


# gráfico de líneas
plt.figure(figsize=(10, 5))
plt.plot(buffers, promedios, marker='o', linestyle='-', color='b')
plt.fill_between(buffers, np.array(promedios) - np.array(desviaciones), np.array(promedios) + np.array(desviaciones), color='blue', alpha=0.2)
plt.xlabel('Tamaño del Buffer (bytes)')
plt.ylabel('Tiempo de Transmisión Promedio (ms)')
plt.title('Tiempo de Transmisión Promedio y Desviación Estándar')
plt.xticks(buffers, [str(b) for b in buffers])
plt.grid(True)
plt.show()