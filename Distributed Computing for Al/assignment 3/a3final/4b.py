import matplotlib.pyplot as plt
import re

# Data extracted from the user's text
data_text = """
16 MPI 1 16
Grid Size       : 1024
Elapsed Time (sec)          : 0.796647
Sustained Gflops Rate       : 71.4611

16 MPI 1 16 Comm disabled
Grid Size       : 1024
Elapsed Time (sec)          : 0.455062
Sustained Gflops Rate       : 125.102

16 MPI 16 1
Grid Size       : 1024
Elapsed Time (sec)          : 2.54051
Sustained Gflops Rate       : 22.4086

16 MPI 16 1 Comm disabled
Grid Size       : 1024
Elapsed Time (sec)          : 0.494037
Sustained Gflops Rate       : 115.233

16 MPI 4 4
Grid Size       : 1024
Elapsed Time (sec)          : 1.26956
Sustained Gflops Rate       : 44.8417

16 MPI 4 4 Comm disabled
Grid Size       : 1024
Elapsed Time (sec)          : 0.460393
Sustained Gflops Rate       : 123.654

16 MPI 8 2
Grid Size       : 1024
Elapsed Time (sec)          : 1.81637
Sustained Gflops Rate       : 31.3424

16 MPI 8 2 Comm disabled
Grid Size       : 1024
Elapsed Time (sec)          : 0.477619
Sustained Gflops Rate       : 119.194

16 MPI 2 8
Grid Size       : 1024
Elapsed Time (sec)          : 0.951403
Sustained Gflops Rate       : 59.8372

16 MPI 2 8 Comm disabled
Grid Size       : 1024
Elapsed Time (sec)          : 0.543196
Sustained Gflops Rate       : 104.804

16 MPI 1 16
Grid Size       : 731
Elapsed Time (sec)          : 0.36795
Sustained Gflops Rate       : 78.8464

16 MPI 1 16 Comm disabled
Grid Size       : 731
Elapsed Time (sec)          : 0.222201
Sustained Gflops Rate       : 130.564

16 MPI 16 1
Grid Size       : 731
Elapsed Time (sec)          : 0.580765
Sustained Gflops Rate       : 49.954

16 MPI 16 1 Comm disabled
Grid Size       : 731
Elapsed Time (sec)          : 0.253392
Sustained Gflops Rate       : 114.493

16 MPI 4 4
Grid Size       : 731
Elapsed Time (sec)          : 0.333569
Sustained Gflops Rate       : 86.9731

16 MPI 4 4 Comm disabled
Grid Size       : 731
Elapsed Time (sec)          : 0.23525
Sustained Gflops Rate       : 123.322

16 MPI 8 2
"""

# Parse the data using regex
pattern = re.compile(r"16 MPI (\d+) (\d+)( Comm disabled)?\nGrid Size       : (\d+)\nElapsed Time \(sec\)          : ([\d.]+)\nSustained Gflops Rate       : ([\d.]+)")

matches = pattern.findall(data_text)

# Prepare data for plotting
grid_sizes = []
elapsed_times = []
gflops_rates = []
geometries = []
for match in matches:
    geometry = "{}x{}".format(match[0], match[1])
    if match[2]:
        geometry += " Comm disabled"
    grid_size = int(match[3])
    elapsed_time = float(match[4])
    gflops_rate = float(match[5])

    geometries.append(geometry)
    grid_sizes.append(grid_size)
    elapsed_times.append(elapsed_time)
    gflops_rates.append(gflops_rate)

# Group the data by grid size for plotting
data_by_grid_size = {}
for i, size in enumerate(grid_sizes):
    if size not in data_by_grid_size:
        data_by_grid_size[size] = {'geometries': [], 'elapsed_times': [], 'gflops_rates': []}
    data_by_grid_size[size]['geometries'].append(geometries[i])
    data_by_grid_size[size]['elapsed_times'].append(elapsed_times[i])
    data_by_grid_size[size]['gflops_rates'].append(gflops_rates[i])


