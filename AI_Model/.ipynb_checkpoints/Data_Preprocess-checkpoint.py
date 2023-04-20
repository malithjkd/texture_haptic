import pandas as pd
import pytz
#from sklearn import *
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

# Reading Different Directories of the data files
csv_dir_1 = '/home/surath/Desktop/FYP 8/Data Read/0.1/sliced_data/S1'
csv_dir_2 = '/home/surath/Desktop/FYP 8/Data Read/0.1/sliced_data/S2'
csv_dir_3 = '/home/surath/Desktop/FYP 8/Data Read/0.1/sliced_data/S3'

# Loading the data from CSV files for each surface into separate DataFrames
# We can use "([pd.read_csv(os.path.join(csv_dir_3, f"Surface_02_{i}.csv")) for i in range () "  to work in any OS
surface1_df = pd.concat([pd.read_csv(f"{csv_dir_1}/Surface_01_{i}.csv") for i in range(1, 91)])
surface2_df = pd.concat([pd.read_csv(f"{csv_dir_2}/Surface_02_{i}.csv") for i in range(1, 107)])
surface3_df = pd.concat([pd.read_csv(f"{csv_dir_3}/Surface_03_{i}.csv") for i in range(1, 105)])


# Combine the three surface DataFrames into a single DataFrame
data_df = pd.concat([surface1_df, surface2_df, surface3_df])

# Separate the features and labels
X = data_df.iloc[:, :-1].values
y = data_df.iloc[:, -1].values

print(X)
print(y)
# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Scale the data
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

