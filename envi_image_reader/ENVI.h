#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

enum DATATYPE
{
	BYTE_8 = 1,
	SIGNED_INTERGER_16 = 2,
	SIGNED_LONG_INTERGER_32 = 3,
	FLOAT_32 = 4,
	DOUBLE_64 = 5,
	UNSIGNED_INTERGER_16 = 12,
	UNSIGNED_LONG_INTERGER_32 = 13,
	SIGNED_LONG_INTERGER_64 = 14,
	UNSIGNED_LONG_INTERGER_64 = 15
};

enum INTERLEAVE
{
	BSQ,
	BIL,
	BIP
};

//头文件信息
struct HeadInfo
{
	string m_description;
	int m_samples;
	int m_lines;
	int m_bands;
	int m_headerOffset;
	string m_fileType;
	DATATYPE m_dataType;
	INTERLEAVE m_interleave;
	string m_sensorType;
	int m_byteOrder;
};

//颜色
struct Color
{
	int r;
	int g;
	int b;
};

//ENVI分类影像头文件新增信息
struct Clas_Header
{
	int classes;
	Color* lookUp;
	string* names;
};

//删除字符串首尾的空格
void Trim(string & s, char ch=' ');

//计算数据类型对应的字节数
int CalculateByte(DATATYPE type);

//根据DATATYPE将char数组转为double
double Byte2Double(const char* ptr, DATATYPE& type);

//根据DATATYPE把double转为char*数组
void Double2Bytes(char* ptr, double& value, DATATYPE & type);

//字符串分裂函数
void Split(vector<string>& splitString, string&s, char ch = ' ');

//ENVI影像类（不使用模板版本）
class Image
{
public:
	Image(string& header, string& fileData);
	virtual ~Image();

	virtual void ShowImage(int bandNum);
	void Save(string& filePath);
protected:
	double** m_data;
	HeadInfo m_headInfo;
	double* m_bandMax;
	double* m_bandMin;
	int m_bytesPerPixel;

	void ReaderHeader(string& header);
	void ReadBinaryData(string & fileName);
	void Initialized();
	virtual void SaveHeader(const string& fileName);
	void SaveData(const string& fileName);
};

//ENVI分类影像类
class Class_Image: public Image
{
public:
	Class_Image(string& header, string& fileData);
	virtual~ Class_Image();
	virtual void ShowImage(int bandNum);
private:
	Clas_Header m_clasHeader;

	void ReadClasHeader(string& fileName);
	virtual void SaveHeader(const string& fileName);
};