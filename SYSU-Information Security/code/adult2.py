import pandas as pd
import numpy as np
from sklearn.metrics import mean_squared_error
import matplotlib.pyplot as plt

# Load the Adult dataset
url = 'https://archive.ics.uci.edu/ml/machine-learning-databases/adult/adult.data'
columns = ['age', 'workclass', 'fnlwgt', 'education', 'education-num',
           'marital-status', 'occupation', 'relationship', 'race', 'sex',
           'capital-gain', 'capital-loss', 'hours-per-week', 'native-country', 'income']

# Load the dataset, handle missing values represented by ' ?'
df = pd.read_csv(url, header=None, names=columns, na_values=' ?')
df = df.dropna()

# Keep only the relevant columns
df = df[['income', 'education-num', 'hours-per-week']]

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

# Specify the numerical columns to which we will add noise
numerical_columns = ['education-num', 'hours-per-week']

# Apply differential privacy to the dataset
# Sensitivity is the range of each feature
sensitivities = {col: df[col].max() - df[col].min() for col in numerical_columns}
epsilons = [0.1,0.11,0.15,0.2,0.3,0.4, 0.5,0.6,0.7,0.8,0.9, 1.0]  # Different levels of privacy

noisy_dfs = []
for epsilon in epsilons:
    noisy_df = apply_differential_privacy(df, numerical_columns, max(sensitivities.values()), epsilon)
    noisy_dfs.append((epsilon, noisy_df))
    print(f"\nNoisy Dataset with epsilon={epsilon}:")
    print(noisy_df.head())

# Measure data utility
def measure_utility(original_df, noisy_dfs, columns):
    """Measures the utility of the noisy data using Mean Squared Error (MSE)"""
    mse_results = []
    for epsilon, noisy_df in noisy_dfs:
        mse = mean_squared_error(original_df[columns], noisy_df[columns])
        mse_results.append((epsilon, mse))
    return mse_results

mse_results = measure_utility(df, noisy_dfs, numerical_columns)

print("\nMean Squared Error between Original and Noisy Data:")
for epsilon, mse in mse_results:
    print(f"Epsilon={epsilon}, MSE={mse}")

# Plot the trade-off
epsilons, mses = zip(*mse_results)
plt.plot(epsilons, mses, marker='o')
plt.title('Trade-off between Data Utility and Privacy')
plt.xlabel('Epsilon')
plt.ylabel('Mean Squared Error (MSE)')
plt.grid(True)
plt.savefig('adult2.png')
plt.show()
