# image_similarity_64
the project is based on
```
qt5(64bit)+py3(64bit)+opencv3.4.1(64bit)
```
# warning
This project is building.  
该项目仍未完成，  
若需要调试与编译源码工程，  
除了需配置相应环境，  
另外还需要更改[pySimilarityCore.cpp](https://github.com/Narthill/image_similarity_64/blob/master/imageSimilarity_64/pySimilarityCore.cpp)中py文件的路径

## 说明
1. 该项目为实现了部分传统图像相似度方法的GUI软件，包括：  
    1. 基于直方图相似度  
    2. 平均哈希  
    3. 感知哈希  
    4. sift特征  
    5. surf特征 
2. 项目使用c++ qt框架内部启动py解释器调用py3文件，py文件再使用opencv进行图像相似度处理  
3. 由于是调用py解释器执行py文件，故软件的执行效率较低
