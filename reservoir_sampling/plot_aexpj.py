# %%
import numpy as np
import matplotlib.pyplot as plt

n = 1000
n_tries = 10000
k = 800

# def weight_func(i):
#     return np.sin(i/100.0) + 1.0

# def weight_func(i):
#     return n - i

def weight_func(i):
    return i

W = np.sum(weight_func(i) for i in range(n))

data = np.loadtxt("./output_a_expj.txt")

# %%
unique, counts = np.unique(data, return_counts=True)
plt.plot(unique, counts)

x = np.linspace(0, n, 100)
plt.plot(x, weight_func(x) * k*n_tries/W, color='black')
plt.show()

# %%
for row in data:
    unique, counts = np.unique(row, return_counts=True)
    assert np.all(counts == 1)
