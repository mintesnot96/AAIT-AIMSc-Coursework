import matplotlib.pyplot as plt
font = {'family': 'serif','color': 'darkred','weight': 'normal','size': 16,}
# Define the configurations and their corresponding Gflops rates for all geometries
configurations_all = [
    "Serial version", "1 MPI", "2 MPI 1X2", "2 MPI 2X1", "4 MPI 1X4",
    "4 MPI 2X2", "4 MPI 4X1", "8 MPI 1X8", "8 MPI 2X4", "8 MPI 8X1",
    "16 MPI 1X16", "16 MPI 2X8", "16 MPI 4X4", "16 MPI 8X2", "16 MPI 16X1"
]

gflops_rates_all = [
    6.04524, 6.00341, 9.94901, 9.88341, 20.3554,
    17.2998, 13.3983, 34.418, 30.9165, 17.8163,
    48.9201, 30.1401, 38.5679, 45.0308, 20.2911
]

# Define the configurations and their corresponding Gflops rates for 1Xx geometries
configurations_1xx = [
    "Serial version", "Geometry 1x1", "Geometry 1x2", "Geometry 1x4",
    "Geometry 1x8", "Geometry 1x16"
]

gflops_rates_1xx = [
    6.04524, 6.00341, 9.94901, 20.3554, 34.418, 48.9201
]

# Let's create separate plots for the two sets of configurations.

# Set up the figure and axis for the first plot (all configurations)
plt.figure(figsize=(15, 10))

# Plot the line with marker 'o', color 'blue' and line style '-'
plt.plot(configurations_all, gflops_rates_all, marker='o', color='blue', linestyle='-', linewidth=2)

# Setting the title with improved font
plt.title("Strong Scaling - Cardiac Electrophysiology Simulation up to 16 MPI", fontdict=font)

# Setting the x and y labels with improved font
plt.xlabel("Configuration Geometry", fontdict=font)
plt.ylabel("GFLOPS Rate", fontdict=font)

# Rotate the x labels to show them better
plt.xticks(rotation=45, ha='right', fontname='DejaVu Sans', fontsize=12)

# Set the y labels with a specific format and font
plt.yticks(fontname='DejaVu Sans', fontsize=12)

# Enhance the edge of the plot
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)
plt.gca().spines['left'].set_linewidth(1.5)
plt.gca().spines['left'].set_color('black')
plt.gca().spines['bottom'].set_linewidth(1.5)
plt.gca().spines['bottom'].set_color('black')

# Adjust the plot to ensure everything fits without overlap
plt.tight_layout()

# Show the plot with improvements for all configurations
plt.show()

# Now let's create a separate plot for the 1Xx geometries

# Set up the figure and axis for the second plot (1Xx geometries)
plt.figure(figsize=(15, 10))

# Plot the line with marker 'o', color 'red' and line style '-'
plt.plot(configurations_1xx, gflops_rates_1xx, marker='o', color='red', linestyle='-', linewidth=2)

# Setting the title with improved font
plt.title("Strong Scaling - 1Xx Geometry Configurations", fontdict=font)

# Setting the x and y labels with improved font
plt.xlabel("Configuration Geometry", fontdict=font)
plt.ylabel("GFLOPS Rate", fontdict=font)

# Rotate the x labels to show them better
plt.xticks(rotation=45, ha='right', fontname='DejaVu Sans', fontsize=12)

# Set the y labels with a specific format and font
plt.yticks(fontname='DejaVu Sans', fontsize=12)

# Enhance the edge of the plot
plt.gca().spines['top'].set_visible(False)
plt.gca().spines['right'].set_visible(False)
plt.gca().spines['left'].set_linewidth(1.5)
plt.gca().spines['left'].set_color('black')
plt.gca().spines['bottom'].set_linewidth(1.5)
plt.gca().spines['bottom'].set_color('black')

# Adjust the plot to ensure everything fits without overlap
plt.tight_layout()

# Show the plot with improvements for 1Xx geometries
plt.show()
