'''
Created on Oct 27, 2010
Logistic Regression Working Module
@author: Peter
'''
from numpy import *
from sklearn import preprocessing
from sklearn.linear_model import LogisticRegression

def loadDataSet():
    dataMat = []; labelMat = []
    fr = open('./testline.txt')
    for line in fr.readlines():
        lineArr = line.strip().split()
        dataMat.append([1.0, float(lineArr[0]), float(lineArr[1])])
        labelMat.append(int(lineArr[2]))
    return dataMat,labelMat

def sigmoid(inX):
    return 1.0/(1+exp(-inX))

def gradAscent(dataMatIn, classLabels):
    dataMatrix = mat(dataMatIn)             #convert to NumPy matrix
    labelMat = mat(classLabels).transpose() #convert to NumPy matrix
    m,n = shape(dataMatrix)
    alpha = 0.001
    maxCycles = 500
    weights = ones((n,1))
    for k in range(maxCycles):              #heavy on matrix operations
        h = sigmoid(dataMatrix*weights)     #matrix mult
        error = (labelMat - h)              #vector subtraction
        weights = weights + alpha * dataMatrix.transpose()* error #matrix mult
    return weights

def plotBestFit(weights):
    import matplotlib.pyplot as plt
    dataMat,labelMat=loadDataSet()
    dataArr = array(dataMat)
    n = shape(dataArr)[0] 
    xcord1 = []; ycord1 = []
    xcord2 = []; ycord2 = []
    for i in range(n):
        if int(labelMat[i])== 1:
            xcord1.append(dataArr[i,1]); ycord1.append(dataArr[i,2])
        else:
            xcord2.append(dataArr[i,1]); ycord2.append(dataArr[i,2])
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(xcord1, ycord1, s=30, c='red', marker='s')
    ax.scatter(xcord2, ycord2, s=30, c='green')
    x = arange(-3.0, 3.0, 0.1)
    y = (-weights[0]-weights[1]*x)/weights[2]
    ax.plot(x, y)
    plt.xlabel('X1'); plt.ylabel('X2');
    plt.show()

def stocGradAscent0(dataMatrix, classLabels):
    m,n = shape(dataMatrix)
    alpha = 0.01
    weights = ones(n)   #initialize to all ones
    for i in range(m):
        h = sigmoid(sum(dataMatrix[i]*weights))
        error = classLabels[i] - h
        weights = weights + alpha * error * dataMatrix[i]
    return weights

def stocGradAscent1(dataMatrix, classLabels, numIter=150):
    m,n = shape(dataMatrix)
    weights = ones(n)   #initialize to all ones
    for j in range(numIter):
        dataIndex = list(range(m))
        for i in range(m):
            alpha = 4/(1.0+j+i)+0.001    #apha decreases with iteration, does not 
            randIndex = int(random.uniform(0,len(dataIndex)))#go to 0 because of the constant
            h = sigmoid(sum(dataMatrix[randIndex]*weights))
            error = classLabels[randIndex] - h
            weights = weights + alpha * error * dataMatrix[randIndex]
            del(dataIndex[randIndex])
    return weights

def classifyVector(inX, weights):
    prob = sigmoid(sum(inX*weights))
    if prob > 0.5: return 1.0
    else: return 0.0

def colicTest(curclass):
    frTrain = open("./4trainSet.txt"); frTest = open('./4testSet.txt')
    trainingSet = []; trainingLabels = []
    # testingSet = []; testingLabels = []
    for line in frTrain.readlines():
        currLine = line.strip().split('\t')
        lineArr =[]
        for i in range(20):
            lineArr.append(float(currLine[i]))
        trainingSet.append(lineArr)
        if currLine[20] == curclass:
            trainingLabels.append(1.0)
        else:
            trainingLabels.append(0.0)

    # for line in frTest.readlines():
        # currLine = line.strip().split('\t')
        # lineArr =[]
        # for i in range(20):
            # lineArr.append(float(currLine[i]))
        # testingSet.append(lineArr)
        # if currLine[20] == curclass:
            # testingLabels.append(1.0)
        # else:
            # testingLabels.append(0.0)
    trainWeights = stocGradAscent1(array(trainingSet), trainingLabels, 100)
    # scaler= preprocessing.MinMaxScaler(feature_range=(0,1)).fit(trainingSet)
    # trainArr = scaler.transform(trainingSet)
    # testArr = scaler.transform(testingSet)

    # 定义逻辑回归模型
    # model = LogisticRegression(penalty='l2', dual=False, tol=0.0001, C=1.0,
    # fit_intercept=True, intercept_scaling=1, class_weight=None,
    # random_state=None, solver='liblinear', max_iter=100, multi_class='ovr',
    # verbose=0, warm_start=False, n_jobs=1)

    """参数
    ---
        penalty：使用指定正则化项（默认：l2）
        dual: n_samples > n_features取False（默认）
        C：正则化强度的反，值越小正则化强度越大
        n_jobs: 指定线程数
        random_state：随机数生成器
        fit_intercept: 是否需要常量
    """
    '''
        # 拟合模型
        model.fit(trainArr, trainingLabels)
        # 模型预测
        model.predict(testArr)

        # 获得这个模型的参数
        model.get_params()
        # 为模型进行打分
        model.score(data_X, data_y) # 线性回归：R square； 分类问题： acc

    '''
    errorCount = 0; numTestVec = 0.0
    for line in frTest.readlines():
        numTestVec += 1.0
        currLine = line.strip().split('\t')
        lineArr =[]
        for i in range(20):
            lineArr.append(float(currLine[i]))
        if currLine[20] == curclass:
            if int(classifyVector(array(lineArr), trainWeights))!= int(1):
                errorCount += 1
        else:
            if int(classifyVector(array(lineArr), trainWeights))!= int(0):
                errorCount += 1

    errorRate = (float(errorCount)/numTestVec)
    print("%d the error rate of this test is: %f" %(curclass,errorRate))
    print(curclass," => ",trainWeights)
    return errorRate

def multiTest():
    for cc in range(1,5):
        numTests = 3; errorSum=0.0
        for k in range(numTests):
            errorSum += colicTest(cc)
        print("%d after %d iterations the average error rate is: %f" % (cc, numTests, errorSum/float(numTests)))
        
multiTest()
# colicTest(1)
