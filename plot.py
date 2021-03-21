import matplotlib
import matplotlib.pyplot as plt
import numpy as np


labels = ['0', '1', '2', '3'] #'4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23']
gamma0 = [20, 34, 30, 35]
gamma1 = [25, 32, 34, 20]
gamma2 = [12, 13, 23, 55]
gamma3 = [11, 33, 44, 27]
gamma4 = [8, 22, 43, 25]
gamma5 = [43, 56, 43, 22]

x = np.arange(len(labels))  # the label locations
width = 0.05  # the width of the bars

fig, ax = plt.subplots()
rects0 = ax.bar(x - 5*width/2, gamma0, width, label='Gamma 0')
rects1 = ax.bar(x - 3*width/2, gamma1, width, label='Gamma 1')
rects2 = ax.bar(x - width/2, gamma2, width, label='Gamma 2')
rects3 = ax.bar(x + width/2, gamma3, width, label='Gamma 3')
rects4 = ax.bar(x + 3*width/2, gamma4, width, label='Gamma 4')
rects5 = ax.bar(x + 5*width/2, gamma5, width, label='Gamma 5')


# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Scores')
ax.set_title('Scores by group and gender')
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


autolabel(rects0)
autolabel(rects1)
autolabel(rects2)
autolabel(rects3)
autolabel(rects4)
autolabel(rects5)

fig.tight_layout()

plt.show()
