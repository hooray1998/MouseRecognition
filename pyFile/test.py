# -*- coding:utf-8 -*-
import numpy as np

from sklearn import preprocessing

trainDataOg = []
with open("./6trainSet.txt") as f:
    for line in f.readlines():
        trainDataOg.append(line.strip().split('\t'))
testDataOg = []
with open("./6testSet.txt") as f:
    for line in f.readlines():
        testDataOg.append(line.strip().split('\t'))

scaler= preprocessing.MinMaxScaler(feature_range=(0,1)).fit(trainDataOg)
trainArr = scaler.transform(trainDataOg)
testArr = scaler.transform(testDataOg)


