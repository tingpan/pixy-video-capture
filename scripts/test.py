import modshogun as sg

# Create the CDenseFeatures and calculate distance using a certain distance metric
train_set = sg.RealFeatures(dataset_object)
distance = EuclideanDistance(train_set, train_set)


# Train with k
kmeans = KMeans(k, distance)
kmeans.train()


# Get radius and center
c = kmeans.get_cluster_centers()
r = kmeans.get_radiuses()



import modshogun as sg

# Create the CDenseFeatures
train_set = sg.RealFeatures(dataset_object)


# Create feature types and multiclass labels
labels = sg.MulticlassLabels(position_data);


# Create C4.5 tree object
tree = sg.C45ClassifierTree()


# Set training labels
tree.set_labels(labels)
tree.set_feature_types(position_data)


# Train
tree.train(train_set)


