import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv('output.txt', sep=" ")


data_rs = data[ data["method"] == "reservoir_sample" ]
data_std = data[ data["method"] == "reservoir_sample_std" ]
data_std_it = data[ data["method"] == "reservoir_sample_std_iterator" ]


for d in [data_rs, data_std, data_std_it]:
    plt.plot( d["n"], d["time"], marker="o", ls="None", label=d["method"])

plt.legend()
# plt.plot( data_std[-2], data_std[-1] )
# plt.plot( data_std_it[-2], data_std_it[-1] )
plt.show()

