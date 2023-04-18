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
    string name;	//����
    Color rgbValue;//rgbValue
    int numOfPoint;	//����
    int bandCount;	//������
    double** data;		//����

    //���캯��
    Region() { numOfPoint = 0; bandCount = 0; data = nullptr; };
    Region(int pointNum, int bandNum);
    virtual	~Region();  	//��������
    void SetName(const string& name) { this->name = name; }	//��������
    void SetRGBValue(string& rgb);  	//����RGBֵ
    void ReadFromFile(ifstream& fin);   	//���ļ���ȡ
    void SetNumOfPointAndBand(int pointNum, int bandNum);   	//���õ����Ͳ��������ɵ����Ͳ��������Է���data�ռ䣩
    void PrintAverage(vector<double>& averBands);    	//��ӡƽ������
};

class ROI
{
private:
	int _numOfROI;	//ROI����
	Region* _listOfRegion;	//Region���飬�洢ÿ��ROI����Ϣ

public:
	//���캯��
    ROI() { _numOfROI = 0; _listOfRegion = nullptr; };
	ROI(int numOfRoi);
    virtual ~ROI() { delete[] _listOfRegion; };     	//��������
    void ReadFile(string fileName); 	//���ļ���ȡ
    void ComputeAverage(vector<vector<double>>& averBands);     //��ӡƽ������
    void SetClassHeader(Clas_Header& header);
};

