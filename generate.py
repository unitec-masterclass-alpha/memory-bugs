import numpy as np

# Parameters
mean = 50000
std_dev = 10000
num_samples = 1000

# Generate samples
samples = np.random.normal(mean, std_dev, num_samples * 2)  # Generate extra to ensure enough distinct values
samples = np.round(samples).astype(int)  # Round to integers
samples = np.clip(samples, 10000, 99999)  # Clip to five-digit range
samples = np.unique(samples)[:num_samples]  # Ensure distinctness and limit to 1000

# Check if we have enough samples
if len(samples) < num_samples:
    raise ValueError("Could not generate enough distinct samples. Try increasing the multiplier.")

np.random.shuffle(samples)
# Save the full list to a file
np.savetxt("five_digit_numbers.txt", samples, fmt="%d")

print("Full list of 1000 distinct five-digit numbers saved to 'five_digit_numbers.txt'.")
