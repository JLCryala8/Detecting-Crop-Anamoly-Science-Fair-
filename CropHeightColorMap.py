#import time
#import random
import math
#import statistics

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import numpy as np
from PIL import Image
filtered_list = []
new_list = []

# Loading the 1st sheet of the Excel file
df = pd.read_csv(r"C:\Users\Neil Kotia\OneDrive\Science Fair 2026-27\Science Fair 2026 Testing Dataset.csv")
# r"C:\Users\YourName\Downloads\sales_data.csv"
# Science Fair 2026 Testing Dataset

drone_height = int(input("Enter the height of the drone (in cm): "))
intervals = int(input("Enter the number of intervals: "))
print()

for i in range(intervals):
    interval_beginning = int(input("Enter the beginning of the interval (in ms): "))
    interval_end = int(input("Enter the end of the interval (in ms): "))
    # Creating a filtered list of distances based on the specified time interval
    filtered_list = df.loc[df["Time (ms)"].between(interval_beginning, interval_end), "Distance (cm)"]

    # Adding to the list by subtracting the filtered distances from the drone height
    new_list.extend((drone_height - filtered_list).tolist())
    print()

print("\nFiltered List:")
print(*new_list, sep="\n")
input()



# Load the square image
img = Image.open("drone_image.jpg")
width, height = img.size

while math.sqrt(len(new_list)).is_integer() == False:
    print("The length of the list is not a perfect square. Please enter new values.")
    drone_height = int(input("Enter the height of the drone (in cm): "))
    intervals = int(input("Enter the number of intervals: "))
    print()

    for i in range(intervals):
        interval_beginning = int(input("Enter the beginning of the interval (in ms): "))
        interval_end = int(input("Enter the end of the interval (in ms): "))
        # Creating a filtered list of distances based on the specified time interval
        filtered_list = df.loc[df["Time (ms)"].between(interval_beginning, interval_end), "Distance (cm)"]

        # Adding to the list by subtracting the filtered distances from the drone height
        new_list.extend((drone_height - filtered_list).tolist())
        print()
    


# Reshape the list into a square grid
side_length = int(math.sqrt(len(new_list)))
grid = np.array(new_list).reshape((side_length, side_length))

# Flip every odd-indexed row (2nd row, 4th row, etc.) right-to-left
grid[1::2] = grid[1::2, ::-1]

# Create a custom colormap (Red for low values -> Green for high values)
low_color = 'red'
high_color = 'green'
custom_cmap = mcolors.LinearSegmentedColormap.from_list("LowToHigh", [low_color, high_color])

# Plot the image and the heatmap overlay
fig, ax = plt.subplots(figsize=(6, 6))

# Display the base image
ax.imshow(img)

# Overlay the square grid
# 'extent' stretches the grid to match the exact pixel dimensions of the image
heatmap = ax.imshow(
    grid, 
    cmap=custom_cmap, 
    alpha=0.5,                      # Transparency (0.0 = invisible, 1.0 = solid)
    extent=[0, width, height, 0],   # Aligns grid to [left, right, bottom, top]
    interpolation='nearest'         # Keeps each of the 9 cells sharp instead of blurry
)

# Add a colorbar legend and clean up axes
plt.colorbar(heatmap, ax=ax, fraction=0.046, pad=0.04, label="Crop Height (cm)")
ax.axis('off')  # Hide pixel axis ticks

plt.tight_layout()
plt.savefig('output_overlay.png', dpi=300)
plt.show()

input()
