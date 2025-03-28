import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.metrics import mean_squared_error
from sklearn.datasets import fetch_openml

# Load the Adult dataset
adult_data = fetch_openml(name='adult', version=2, as_frame=True)
df = adult_data.frame

# Select relevant columns
df = df[['age', 'hours-per-week']]
print("Original Dataset:")
print(df.head())

# Differential privacy functions
def add_laplace_noise(value, sensitivity, epsilon):
    """Adds Laplace noise to a given value"""
    scale = sensitivity / epsilon
    noise = np.random.laplace(0, scale, 1)
    return value + noise

def apply_differential_privacy(df, columns, sensitivity, epsilon):
    """Applies Laplace noise to the specified columns of the dataframe"""
    noisy_df = df.copy()
    for column in columns:
        noisy_df[column] = noisy_df[column].apply(lambda x: add_laplace_noise(x, sensitivity, epsilon))
    return noisy_df

# Apply differential privacy to the dataset
sensitivity_age = df['age'].max() - df['age'].min()  # Sensitivity for age
sensitivity_hours = df['hours-per-week'].max() - df['hours-per-week'].min()  # Sensitivity for hours-per-week

epsilons = [0.1, 0.5, 1.0]  # Different levels of privacy

noisy_dfs = []
for epsilon in epsilons:
    noisy_df = apply_differential_privacy(df, ['age', 'hours-per-week'], sensitivity_age, epsilon)
    noisy_dfs.append((epsilon, noisy_df))
    print(f"\nNoisy Dataset with epsilon={epsilon}:")
    print(noisy_df.head())

# Measure data utility
def measure_utility(original_df, noisy_dfs, columns):
    """Measures the utility of the noisy data using Mean Squared Error (MSE)"""
    mse_results = []
    for epsilon, noisy_df in noisy_dfs:
        mse_age = mean_squared_error(original_df[columns[0]], noisy_df[columns[0]])
        mse_hours = mean_squared_error(original_df[columns[1]], noisy_df[columns[1]])
        mse_results.append((epsilon, mse_age, mse_hours))
    return mse_results

mse_results = measure_utility(df, noisy_dfs, ['age', 'hours-per-week'])

print("\nMean Squared Error between Original and Noisy Data:")
for epsilon, mse_age, mse_hours in mse_results:
    print(f"Epsilon={epsilon}, MSE (Age)={mse_age}, MSE (Hours-per-week)={mse_hours}")

# Plot the trade-off
epsilons, mses_age, mses_hours = zip(*mse_results)
plt.plot(epsilons, mses_age, marker='o', label='Age')
plt.plot(epsilons, mses_hours, marker='o', label='Hours-per-week')
plt.title('Trade-off between Data Utility and Privacy')
plt.xlabel('Epsilon')
plt.ylabel('Mean Squared Error (MSE)')
plt.legend()
plt.grid(True)
plt.savefig('adult.png')
