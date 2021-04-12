import matplotlib
import matplotlib.pyplot as plt
import numpy as np

x = [0,1,2,3,4,5]

t_risp =[8.14215304917627, 1.12805484902849, 1.25788784841984, 0.976513056934154, 0.724418491273482, 0.829639133444647]
plt.ylabel('Average Response Time')
plt.xlabel('Gamma')
plt.plot(x, t_risp)

plt.show()


drop =[0.140911293549711, 0.016634773534776, 0.008873626600143, 0.004829451013139, 0.003056034919686, 0.002676483843622]
plt.ylabel('Average Drop Rate')
plt.xlabel('Gamma')
plt.plot(x,drop)
plt.show()
