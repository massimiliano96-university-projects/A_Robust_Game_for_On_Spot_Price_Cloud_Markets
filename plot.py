import matplotlib
import matplotlib.pyplot as plt
import numpy as np


labels = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23']
gamma0 = [2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 3, 3, 4, 3, 3, 3, 3, 3, 3]
gamma1 = [4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 6, 5, 5, 5, 5, 5, 5]
gamma2 = [5, 5, 4, 4, 4, 3, 3, 3, 3, 4, 4, 5, 5, 5, 5, 6, 6, 7, 7, 6, 7, 6, 7, 7]
gamma3 = [6, 6, 5, 5, 4, 4, 4, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7, 9, 8, 7, 8, 8, 8, 8]
gamma4 = [7, 7, 5, 5, 5, 4, 4, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 10, 9, 8, 9, 9, 9, 9]
gamma5 = [7, 7, 6, 6, 5, 5, 5, 5, 5, 5, 6, 7, 7, 8, 7, 8, 9, 11, 10, 9, 10, 9, 10, 10]

x = np.arange(len(labels))  # the label locations
width = 0.13  # the width of the bars

fig, ax = plt.subplots()
rects0 = ax.bar(x - 5*width/2, gamma0, width, label='Gamma 0')
rects1 = ax.bar(x - 3*width/2, gamma1, width, label='Gamma 1')
rects2 = ax.bar(x - width/2, gamma2, width, label='Gamma 2')
rects3 = ax.bar(x + width/2, gamma3, width, label='Gamma 3')
rects4 = ax.bar(x + 3*width/2, gamma4, width, label='Gamma 4')
rects5 = ax.bar(x + 5*width/2, gamma5, width, label='Gamma 5')


# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Number of VMs')
ax.set_xlabel('Hours of the day')
ax.set_title('Daily number of VMs')
ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.legend()


def autolabel(rects):
    """Attach a text label above each bar in *rects*, displaying its height."""
    for rect in rects:
        height = rect.get_height()
        ax.annotate('{}'.format(height),
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 1),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')


#autolabel(rects0)
#autolabel(rects1)
#autolabel(rects2)
#autolabel(rects3)
#autolabel(rects4)
#autolabel(rects5)

fig.tight_layout()

plt.show()
