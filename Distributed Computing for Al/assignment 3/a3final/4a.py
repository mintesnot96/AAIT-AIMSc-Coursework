import matplotlib.pyplot as plt
font = {'family': 'serif',
        'color':  'darkred',
        'weight': 'normal',
        'size': 16,
        }


# Redefine the configurations and their corresponding Gflops rates excluding the 32 MPI configurations
configurations = [
    "Serial version", "1 MPI", "2 MPI 1X2", "2 MPI 2X1", "4 MPI 1X4",
    "4 MPI 2X2", "4 MPI 4X1", "8 MPI 1X8", "8 MPI 2X4", "8 MPI 8X1",
    "16 MPI 1X16", "16 MPI 2X8", "16 MPI 4X4", "16 MPI 8X2", "16 MPI 16X1"
]

gflops_rates = [7.159807, 7.117977, 11.063576999999999, 10.997976999999999,
21.469967, 18.414367000000002, 14.512867, 35.532567, 32.031067, 18.930867,
50.034667, 31.254667, 39.682467,46.145367, 21.405667]
# Plotting the graph again with the configurations up to MPI 16 and without the grid
plt.figure(figsize=(15, 10))

# Plot the line with marker 'o', color 'blue' and line style '-'
plt.plot(configurations, gflops_rates, marker='o', color='blue', linestyle='-', linewidth=2)

# Setting the title with improved font
plt.title("Strong Scaling - Cardiac Electrophysiology Simulation up to 16 MPI", fontdict=font)

# Setting the x and y labels with improved font
plt.xlabel("Configuration Geometry", fontdict=font)
plt.ylabel("GFLOPS Rate", fontdict=font)

# Rotate the x labels to show them better
plt.xticks(rotation=45, ha='right', fontname='DejaVu Sans', fontsize=12)

# Set the y labels with a specific format and font
plt.yticks(fontname='DejaVu Sans', fontsize=12)

# Set the background color
plt.gca().set_facecolor('whitesmoke')

# Enhance the edge of the plot
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)
plt.gca().spines['left'].set_linewidth(1.5)
plt.gca().spines['left'].set_color('black')
plt.gca().spines['bottom'].set_linewidth(1.5)
plt.gca().spines['bottom'].set_color('black')

# Adjust the plot to ensure everything fits without overlap
plt.tight_layout()

# Show the plot with improvements
plt.show()
