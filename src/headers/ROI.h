#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Function.h"

using namespace std;

class Region
{
    friend class ROI;
private:
    string name;	//名称
    Color rgbValue;//rgbValue
    int numOfPoint;	//点数
    int bandCount;	//波段数
    double** data;		//数据

    //构造函数
    Region() { numOfPoint = 0; bandCount = 0; data = nullptr; };
    Region(int pointNum, int bandNum);
    virtual	~Region();  	//析构函数
    void SetName(const string& name) { this->name = name; }	//设置名称
    void SetRGBValue(string& rgb);  	//设置RGB值
    void ReadFromFile(ifstream& fin);   	//从文件读取
    void SetNumOfPointAndBand(int pointNum, int bandNum);   	//设置点数和波段数（由点数和波段数可以分配data空间）
    void PrintAverage(vector<double>& averBands);    	//打印平均光谱
};

class ROI
{
private:
	int _numOfROI;	//ROI个数
	Region* _listOfRegion;	//Region数组，存储每个ROI的信息

public:
	//构造函数
    ROI() { _numOfROI = 0; _listOfRegion = nullptr; };
	ROI(int numOfRoi);
    virtual ~ROI() { delete[] _listOfRegion; };     	//析构函数
    void ReadFile(string fileName); 	//从文件读取
    void ComputeAverage(vector<vector<double>>& averBands);     //打印平均光谱
    void SetClassHeader(Clas_Header& header);
};

