import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv('output.txt', sep=" ")


data_rs = data[ data["method"] == "reservoir_sample" ]
data_std = data[ data["method"] == "reservoir_sample_std" ]
data_std_it = data[ data["method"] == "reservoir_sample_std_iterator" ]

for d in [data_rs, data_std, data_std_it]:
    plt.plot( d["n"], d["time"], marker="o", ls="None", label=d["method"].iloc[0])

plt.ylabel("k")
plt.xlabel("n")
plt.legend()
plt.show()


for d in [data_rs, data_std, data_std_it]:
    d = d[ d["n"] == max(d["n"]) ]
    plt.plot( d["k"], d["time"], marker="o", ls="-", label=d["method"].iloc[0])
plt.xlabel("k")

plt.show()