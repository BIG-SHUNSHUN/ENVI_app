#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>
#include <sstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//颜色
struct Color
{
    int r;
    int g;
    int b;
};

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

enum IMAGETYPE
{
    ENVI_STANDARD,
    ENVI_CLASSIFICATION
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
    vector<string> m_bandNames;
};

//ENVI分类影像头文件新增信息
struct Clas_Header
{
    int classes;
    Color* lookUp;
    string* names;
};

//计算数据类型对应的字节数
int CalculateByte(DATATYPE type);

//根据DATATYPE将char数组转为double
double Byte2Double(const char* ptr, DATATYPE& type);

//根据DATATYPE把double转为char*数组
void Double2Bytes(char* ptr, double& value, DATATYPE & type);

//删除字符串首尾的空格
void Trim(string & s, char ch=' ');

//字符串分裂函数
void Split(vector<string>& splitString, string&s, char ch = ' ');

DATATYPE JudgeDatatype(Mat img);

#endif // FUNCTION_H
