#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <QPainter>
#include "Function.h"
#include "ROI.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

//ENVIӰ���ࣨ��ʹ��ģ��汾��
class Image
{
public:
    Image();
	Image(string& header, string& fileData);
    Image(Mat img);
	virtual ~Image();

    virtual void ShowImage(QPainter* painter, int* bands) const;
    void Save(string filePath);
    const HeadInfo& GetHeaderInfo() const;
    string GetFileName() const;
    void StructureData(vector<vector<double>>& structuredData);
    vector<double> operator[](int n) const;    //����[]���������ȡĳ����Ԫ�Ĺ�������
    void SetBandName(string bandName);
    void SetFileName(string fileName);
    int GetId() const;
    Mat ConvertToOpencvMat(int band) const;     //ת��ΪMat
    Mat ConvertToOpencvMat() const;     //ת��ΪMat 8UC3

protected:
	double** m_data;
	HeadInfo m_headInfo;
	double* m_bandMax;
	double* m_bandMin;
	int m_bytesPerPixel;
    string m_fileName;
    int m_id;

    static int num;

	void ReaderHeader(string& header);
	void ReadBinaryData(string & fileName);
	void Initialized();
	virtual void SaveHeader(const string& fileName);
	void SaveData(const string& fileName);
    virtual void ShowImage(QPainter* painter, int bandNum) const;
};

//ENVI����Ӱ����
class Class_Image: public Image
{
public:
    Class_Image(uchar* data, int rows, int cols, ROI* roi);    //�������й���Ӱ��
	Class_Image(string& header, string& fileData);
	virtual~ Class_Image();
    virtual void ShowImage(QPainter* painter, int* bands) const;
    int GetClasses() const;    //��������
    const Clas_Header& GetClasHeader() const;
private:
	Clas_Header m_clasHeader;

	void ReadClasHeader(string& fileName);
	virtual void SaveHeader(const string& fileName);
    virtual void ShowImage(QPainter* painter, int bandNum) const;
    void SetClasHeader(ROI* roi);
};
