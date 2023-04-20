import pandas as pd
# from sklearn.model_selection import train_test_split
# from sklearn.linear_model import LogisticRegression
# from sklearn.metrics import accuracy_score
# import joblib
print("1234")
# Reading Different Directories of the data files
csv_dir_1 = '/home/surath/Desktop/FYP 8/Data Read/0.1/sliced_data/S1'
csv_dir_2 = '/home/surath/Desktop/FYP 8/Data Read/0.1/sliced_data/S2'
csv_dir_3 = '/home/surath/Desktop/FYP 8/Data Read/0.1/sliced_data/S3'

# Load the data from CSV files for each surface
print("A")
surface1_df  = []
for i in range(1, 91):
    filename = f"{csv_dir_1}/Surface_01_{i}.csv"
    df = pd.read_csv(filename)
    surface1_df.append(df)
surface1_df = pd.concat(surface1_df)

print(surface1_df)
print("B")
surface2_df= []
for i in range(1, 107):
    filename = f'Surface_02_{i}.csv'
    df = pd.read_csv(filename)
    surface2_df.append(df)
surface2_df = pd.concat(surface2_df)

surface3_df = []
for i in range(1, 105):
    filename = f'Surface_03_{i}.csv'
    df = pd.read_csv(filename)
    surface3_df.append(df)
surface3_df = pd.concat(surface3_df)

# Combine the data from all three surfaces
all_data = pd.concat([surface1_df, surface2_df, surface3_df])

# # Separate the features and labels
# X = all_data.drop('surface_type', axis=1)
# y = all_data['surface_type']

# # Split the data into training and testing sets
# X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# # Choose a machine learning algorithm
# model = LogisticRegression()

# # Train the model on the training set
# model.fit(X_train, y_train)

# # Evaluate the model on the testing set
# y_pred = model.predict(X_test)
# accuracy = accuracy_score(y_test, y_pred)
# print(f"Accuracy: {accuracy}")

# # Save the trained model
# joblib.dump(model, 'surface_classification_model.joblib')
