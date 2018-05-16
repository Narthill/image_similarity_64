# coding=utf-8
# 利用python实现多种方法来实现图像识别

import os
import sys

import numpy as np
#from matplotlib import pyplot as plt

import cv2


# 最简单的以灰度直方图作为相似比较的实现
class similarityWorker(object):
    '该类用于相似度比较'
    def __init__(self, image1Path, image2Path):
        self.__imageOne = cv2.imread(image1Path)
        self.__imageTwo = cv2.imread(image2Path)

    def classify_gray_hist(self, size=(256, 256)):
        # 先计算直方图
        # 几个参数必须用方括号括起来
        # 这里直接用灰度图计算直方图，所以是使用第一个通道，
        # 也可以进行通道分离后，得到多个通道的直方图
        # bins 取为16
        
        image1 = cv2.resize(self.__imageOne, size)
        image2 = cv2.resize(self.__imageTwo, size)
        image1 = cv2.cvtColor(image1, cv2.COLOR_BGR2GRAY)
        image2 = cv2.cvtColor(image2, cv2.COLOR_BGR2GRAY)
        hist1 = cv2.calcHist([image1], [0], None, [256], [0.0, 255.0])
        hist2 = cv2.calcHist([image2], [0], None, [256], [0.0, 255.0])
        # 可以比较下直方图
        # plt.plot(range(256), hist1, 'r')
        # plt.plot(range(256), hist2, 'b')
        # plt.show()
        # 计算直方图的重合度
        degree = 0
        for i in range(len(hist1)):
            if hist1[i] != hist2[i]:
                degree = degree + \
                    (1 - abs(hist1[i]-hist2[i])/max(hist1[i], hist2[i]))
            else:
                degree = degree + 1
        degree = degree/len(hist1)
        return degree

    # 通过得到每个通道的直方图来计算相似度


    def classify_hist_with_split(self,size=(256, 256)):
        # 将图像resize后，分离为三个通道，再计算每个通道的相似值
        image1 = cv2.resize(self.__imageOne, size)
        image2 = cv2.resize(self.__imageTwo, size)
        sub_image1 = cv2.split(image1)
        sub_image2 = cv2.split(image2)
        sub_data = 0
        for im1, im2 in zip(sub_image1, sub_image2):
            sub_data += self.calculate(im1, im2)
        sub_data = sub_data/3
        return sub_data

    # 计算单通道的直方图的相似值

    #!实例方法只能被实例对象调用，静态方法(由@staticmethod装饰的方法)、类方法(由@classmethod装饰的方法)，可以被类或类的实例对象调用。
    #!实例方法，第一个参数必须要默认传实例对象，一般习惯用self。
    #!静态方法，参数没有要求。
    #!类方法，第一个参数必须要默认传类，一般习惯用cls。
    @classmethod
    def calculate(cls,image1,image2):
        hist1 = cv2.calcHist([image1], [0], None, [256], [0.0, 255.0])
        hist2 = cv2.calcHist([image2], [0], None, [256], [0.0, 255.0])
        # 计算直方图的重合度
        degree = 0
        for i in range(len(hist1)):
            if hist1[i] != hist2[i]:
                degree = degree + \
                    (1 - abs(hist1[i]-hist2[i])/max(hist1[i], hist2[i]))
            else:
                degree = degree + 1
        degree = degree/len(hist1)
        return degree

    # 平均哈希算法计算

    
    def classify_aHash(self):
        image1 = cv2.resize(self.__imageOne, (8, 8))
        image2 = cv2.resize(self.__imageTwo, (8, 8))
        gray1 = cv2.cvtColor(image1, cv2.COLOR_BGR2GRAY)
        gray2 = cv2.cvtColor(image2, cv2.COLOR_BGR2GRAY)
        hash1 = self.getHash(gray1)
        hash2 = self.getHash(gray2)
        return self.Hamming_distance(hash1, hash2)


    def classify_pHash(self):
        image1 = cv2.resize(self.__imageOne, (32, 32))
        image2 = cv2.resize(self.__imageTwo, (32, 32))
        gray1 = cv2.cvtColor(image1, cv2.COLOR_BGR2GRAY)
        gray2 = cv2.cvtColor(image2, cv2.COLOR_BGR2GRAY)
        # 将灰度图转为浮点型，再进行dct变换
        dct1 = cv2.dct(np.float32(gray1))
        dct2 = cv2.dct(np.float32(gray2))
        # 取左上角的8*8，这些代表图片的最低频率
        # 这个操作等价于c++中利用opencv实现的掩码操作
        # 在python中进行掩码操作，可以直接这样取出图像矩阵的某一部分
        dct1_roi = dct1[0:8, 0:8]
        dct2_roi = dct2[0:8, 0:8]
        hash1 = self.getHash(dct1_roi)
        hash2 = self.getHash(dct2_roi)
        return self.Hamming_distance(hash1, hash2)

    # 输入灰度图，返回hash

    # 计算平均值： 计算进行灰度处理后图片的所有像素点的平均值。
    # 比较像素灰度值：遍历灰度图片每一个像素，如果大于平均值记录为1，否则为0.
    # 得到信息指纹：组合64个bit位，顺序随意保持一致性即可。
    @classmethod
    def getHash(self,image):
        avreage = np.mean(image)
        hash = []
        for i in range(image.shape[0]):#行数
            for j in range(image.shape[1]):#列数
                if image[i, j] > avreage:
                    hash.append(1)
                else:
                    hash.append(0)
        return hash


    # 计算汉明距离
    @classmethod
    def Hamming_distance(self,hash1, hash2):
        num = 0
        for index in range(len(hash1)):
            if hash1[index] != hash2[index]:
                num += 1
        return num

def doSimilarity_classify_gray_hist(image1Path,image2Path):
    # image1Path=image1Path.decode('gbk');
    # image2Path=image2Path.decode('gbk');
    sm=similarityWorker(image1Path,image2Path)
    #print "similarityWorker.__doc__:", (similarityWorker.__doc__).decode('utf-8').encode('gbk')#python设置为utf-8编码，要windows cmd正常输出需要解码之后再gbk编码
    degree = sm.classify_gray_hist()
    print ("基于灰度化直方图相似度:",degree)
    return degree

def doSimilarity_classify_hist_with_split(image1Path,image2Path):
    sm=similarityWorker(image1Path,image2Path)
    degree = sm.classify_hist_with_split()
    print ("分离各通道直方图后的相似度:",degree)
    return degree

def doSimilarity_classify_aHash(image1Path,image2Path):
    sm=similarityWorker(image1Path,image2Path)
    degree = sm.classify_aHash()
    print ('aHash:',degree)
    return degree

def doSimilarity_classify_pHash(image1Path,image2Path):
    sm=similarityWorker(image1Path,image2Path)
    degree = sm.classify_pHash()
    print ('pHash:',degree)
    return degree

#D:\Software\Anaconda2\Scripts\pyinstaller -F E:\Item\Python\imageSim\seemImg.py
if __name__ == '__main__':
   doSimilarity_classify_gray_hist("imag/1.jpg","imag/0.jpg");
   doSimilarity_classify_aHash("imag/0.jpg","imag/1.jpg");
