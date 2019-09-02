from windrose import WindroseAxes
from matplotlib import pyplot as plt
import matplotlib.cm as cm
import numpy as np
import pandas as pd
import sys


try:
	filename = str(sys.argv[1])
	titre = str(sys.argv[2])
	out = str(sys.argv[3])
except :
	print('Nous ne pouvons pas recuperer les argument : plot.py')
	sys.exit(2)
try :
	datapd = pd.read_csv(filename, encoding='utf-8')
	datanp = datapd.values
	ws = datanp[:,0]
	wd = datanp[:,1]
except :
	print('arguments exception, revoir ton fichier de donnees (nom, ...) : plot.py')
	sys.exit(2)

plt.hist([0, 1])
plt.close()

ax = WindroseAxes.from_ax()
ax.bar(wd, ws, normed=True, opening=0.8, edgecolor='white')

ax.set_legend()
ax.legend(loc=(-0.12, 0.75), title="legend", fontsize=9)
ax.get_legend().get_title().set_fontsize('9')
plt.title(titre, y=1.08, fontsize=16)

plt.savefig(out)
plt.close()

