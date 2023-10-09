#%%
import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt( "./output_a_expj.txt" )

# %%

unique, counts = np.unique(data, return_counts=True)
plt.plot(unique, counts)

# %%
for row in data:
    unique, counts = np.unique(row, return_counts=True)
    assert np.all( counts == 1 )
