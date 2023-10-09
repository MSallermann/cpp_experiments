# %%
import numpy as np
import matplotlib.pyplot as plt


N = 1000


def weight_func(i):
    return np.sin(i/100.0) + 1.0


W = np.sum(weight_func(i) for i in range(N))

data = np.loadtxt("./output_a_expj.txt")

# %%
unique, counts = np.unique(data, return_counts=True)
plt.plot(unique, counts)

x = np.linspace(0, N, 100)
plt.plot(x, weight_func(x) * 1000000/W, color='black')
plt.show()

# %%
for row in data:
    unique, counts = np.unique(row, return_counts=True)
    assert np.all(counts == 1)
