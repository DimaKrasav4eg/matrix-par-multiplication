import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Circle, Rectangle

with open('graph_points.txt', 'r') as file:
	data = file.read().split()

y = []
for i in range(2):
	row = []
	logrow = []
	for j in range(len(data)//2):
		index = i * (len(data) // 2) + j
		row.append(float(data[index]))
	y.append(row)

x = [i+1 for i in range(len(y[0]))]
# print(y)
# set the size of the graph 
plt.figure(figsize=(8, 8))

plt.plot(x, y[0], label='naive_par', color='g', marker='o')
plt.plot(x, y[2], label='shtassen_par', color='y', marker='o')


plt.xlabel('number of logical cores')
plt.ylabel('Time, s')
plt.title('The dependence of the execution time on the number of logical cores')
plt.legend()
plt.grid(True)

plt.savefig('images/graph.png')
plt.show()