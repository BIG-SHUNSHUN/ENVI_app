#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
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

void Trim(string & s);

//ENVI影像类（模板类版本）
template<typename T> class Image
{
public:
	Image(HeadInfo& headInfo, string& fileData);
	~Image() {};

	void ShowImage(int bandNum);
private:
	vector<vector<T>> m_data;
	HeadInfo m_headInfo;
	vector<T> m_bandMax;
	vector<T> m_bandMin;

	void ReadBinaryData(string & fileName);
};

HeadInfo ReadHeaderInfo(string & fileHeader);


template<typename T>
Image<T>::Image(HeadInfo& headInfo, string& fileData)
	:m_data(headInfo.m_bands, vector<T>(headInfo.m_lines*headInfo.m_samples)),
	m_bandMax(headInfo.m_bands),
	m_bandMin(headInfo.m_bands)
{
	m_headInfo = headInfo;
	ReadBinaryData(fileData);
}

template<typename T>
void Image<T>::ShowImage(int bandNum)
{
	int rows = m_headInfo.m_lines;
	int cols = m_headInfo.m_samples;
	Mat mat(rows, cols, CV_8UC1);
	//找到最大和最小值
	T max_value = m_bandMax[bandNum], min_value = m_bandMin[bandNum];
	//拉伸到0-255
	for (int i = 0; i < rows; i++)
	{
		uchar * ptr = mat.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			ptr[j] = (m_data[bandNum][i*cols + j] - min_value) / (double)(max_value - min_value) * 255;
		}
	}
	imshow("演示", mat);
	waitKey(0);
}

template<typename T>
void Image<T>::ReadBinaryData(string & fileName)
{
	ifstream fin(fileName, ios::in | ios::binary);
	//先读取header_offset个字节
	char temp[128];
	fin.read(temp, m_headInfo.m_headerOffset);
	//再读取所有数据
	int rows = m_headInfo.m_lines;
	int cols = m_headInfo.m_samples;	
	T * data = new T[rows*cols*m_headInfo.m_bands];
	fin.read((char*)data, rows*cols*m_headInfo.m_bands * sizeof(T));
	fin.close();
	T* data_temp = data;
	//如果是BSQ存储
	if (m_headInfo.m_interleave == INTERLEAVE::BSQ)
	{
		for (int i = 0; i < m_headInfo.m_bands; i++)
		{
			T max_value, min_value;
			for (int row = 0; row < rows; row++)
			{
				for (int col = 0; col < cols; col++)
				{
					m_data[i][row*cols + col] = *data_temp;
					data_temp++;
					if (row == 0 && col == 0)
					{
						max_value = m_data[i][row*cols + col];
						min_value = m_data[i][row*cols + col];
					}
					else
					{
						max_value = max_value > m_data[i][row*cols + col] ? max_value : m_data[i][row*cols + col];
						min_value = min_value < m_data[i][row*cols + col] ? min_value : m_data[i][row*cols + col];
					}
				}
			}
			m_bandMax[i] = max_value;
			m_bandMin[i] = min_value;
		}
	}
	if (m_headInfo.m_interleave == INTERLEAVE::BIP)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				for (int k = 0; k < m_headInfo.m_bands; k++)
				{
					m_data[k][i*cols + j] = *data_temp;
					data_temp++;
				}
				for (int k = 0; k < m_headInfo.m_bands; k++)
				{
					if (i == 0 && j == 0)
					{
						m_bandMax[k] = m_data[k][0];
						m_bandMin[k] = m_data[k][0];
					}
					else
					{
						m_bandMax[k] = m_bandMax[k] > m_data[k][i*cols + j] ? m_bandMax[k] : m_data[k][i*cols + j];
						m_bandMin[k] = m_bandMin[k] < m_data[k][i*cols + j] ? m_bandMin[k] : m_data[k][i*cols + j];
					}
				}
			}
		}
	}
	delete[] data;
}