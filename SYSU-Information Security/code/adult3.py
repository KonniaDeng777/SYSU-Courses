
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import LabelEncoder

# Load the dataset
url = "https://archive.ics.uci.edu/ml/machine-learning-databases/adult/adult.data"
column_names = ["age", "workclass", "fnlwgt", "education", "education_num",
                "marital_status", "occupation", "relationship", "race",
                "sex", "capital_gain", "capital_loss", "hours_per_week", "native_country", "income"]
data = pd.read_csv(url, header=None, names=column_names, na_values=' ?', skipinitialspace=True)

# Drop rows with missing values
data.dropna(inplace=True)

# Encode categorical variables
label_encoders = {}
for column in data.select_dtypes(include=['object']).columns:
    label_encoders[column] = LabelEncoder()
    data[column] = label_encoders[column].fit_transform(data[column])

# Display the first few rows of the dataset
print(data.head())


# Function to add Laplace noise for differential privacy
def laplace_mechanism(value, sensitivity, epsilon):
    noise = np.random.laplace(0, sensitivity / epsilon, 1)[0]
    return value + noise


# Apply differential privacy to the dataset
epsilon = 1.0  # Privacy budget
sensitivity = 0.1  # Sensitivity of the query

dp_data = data.copy()
for column in dp_data.columns:
    if np.issubdtype(dp_data[column].dtype, np.number):
        dp_data[column] = dp_data[column].apply(lambda x: laplace_mechanism(x, sensitivity, epsilon))


# Measure the trade-off between data utility and privacy protection
def measure_utility(original, dp):
    mse = ((original - dp) ** 2).mean()
    return mse


utilities = {column: measure_utility(data[column], dp_data[column]) for column in data.columns}

# Print utility metrics for each column
for column, utility in utilities.items():
    print(f"{column} utility (MSE): {utility}")


# Plot the original and DP datasets for selected columns
def plot_histograms(original, dp, columns, bins=15):
    plt.figure(figsize=(12, 6 * len(columns) // 2))
    for i, column in enumerate(columns, 1):
        plt.subplot(len(columns) // 2, 2, i)
        plt.hist(original[column], bins=bins, alpha=0.5, label='Original')
        plt.hist(dp[column], bins=bins, alpha=0.5, label='DP')
        plt.title(f'{column} Distribution')
        plt.legend()
    plt.tight_layout()
    plt.savefig('his-s01.png')
    plt.show()


# Plot histograms for selected columns
plot_histograms(data, dp_data, ['age', 'education_num', 'hours_per_week', 'workclass'])


# Plot MSE for different columns
def plot_mse(utilities):
    columns = list(utilities.keys())
    mse_values = list(utilities.values())

    plt.figure(figsize=(12, 6))
    plt.bar(columns, mse_values, color='skyblue')
    plt.xlabel('Columns')
    plt.ylabel('Mean Squared Error (MSE)')
    plt.title('MSE of Different Columns After Applying Differential Privacy')
    plt.xticks(rotation=90)
    plt.tight_layout()
    plt.savefig('mse-s01.png')
    plt.show()


# Plot MSE for all columns
plot_mse(utilities)
