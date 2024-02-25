import matplotlib.pyplot as plt

# Configuration data
configurations = ['Serial', '1x1', '1x2', '1x4', '1x8', '1x16']
gflops_rates = [8.10818, 11.99572, 19.32409, 44.8672, 88.1378, 61.0013]

# Plotting the graph
plt.figure(figsize=(10, 6))
plt.plot(configurations, gflops_rates, marker='o', linestyle='-', color='b', linewidth=2, label='GFLOPS Rate')
plt.title('1D Implementation Performance (On my PC - capable of 8 processes)', fontsize=16)
plt.xlabel('Configuration', fontsize=14)
plt.ylabel('GFLOPS Rate', fontsize=14)
plt.xticks(rotation=45, fontsize=12)
plt.yticks(fontsize=12)
plt.legend(fontsize=12)
plt.tight_layout()

# Annotating each point with its value
for i, rate in enumerate(gflops_rates):
    plt.text(i, rate, str(round(rate, 2)), ha='center', va='bottom', fontsize=10)

# Show plot
plt.show()
