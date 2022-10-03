#include "ROI.h"
#include <QtDebug>>

Region::Region(int pointNum, int bandNum)
{
	SetNumOfPointAndBand(pointNum, bandNum);
}

Region::~Region()
{
	for (int i = 0; i < numOfPoint; i++)
	{
		delete[] data[i];
	}
    delete[]data;
}

void Region::SetRGBValue(string &rgb)
{
    vector<string> splitString;
    Split(splitString, rgb, ',');
    rgbValue.r = atoi(splitString[0].c_str());
    rgbValue.g = atoi(splitString[1].c_str());
    rgbValue.b = atoi(splitString[2].c_str());
}

void Region::PrintAverage(vector<double>& averBands)
{
    double* aver = new double[bandCount - 3];
    //初始化为0
    for(int i = 0; i < bandCount - 3; i++)
    {
        aver[i] = 0;
    }
	//累加
	for (int i = 0; i < numOfPoint; i++)
	{
        for (int j = 3; j < bandCount; j++)
		{
            aver[j - 3] += data[i][j];
		}
	}
	//平均并输出
    qDebug() <<  name.c_str() <<" 平均光谱:";
    for (int i = 0; i < bandCount - 3; i++)
	{
        aver[i] = aver[i] / numOfPoint;
        averBands.push_back(aver[i]);
        qDebug() << "    Band " << i + 1 << " :" << averBands[i];
	}

   delete[] aver;
}

void Region::ReadFromFile(ifstream & fin)
{
	string data;
	//每个点循环读取
	for (int i = 0; i < numOfPoint; i++)
	{
		//读取一行
		getline(fin, data);
		//存储split之后的字符串，用于转成int
		vector<string> stringSplit;
        Split(stringSplit, data);
		for (int j = 0; j < stringSplit.size(); j++)
		{
            this->data[i][j] = atoi(stringSplit[j].c_str());
		}
	}
}

void Region::SetNumOfPointAndBand(int pointNum, int bandNum)
{
	numOfPoint = pointNum;
	bandCount = bandNum;
	//分配数据空间
	data = new double*[pointNum];
	for (int i = 0; i < pointNum; i++)
	{
        data[i] = new double[bandNum];
	}
}

ROI::ROI(int numOfRoi)
{
	_numOfROI = numOfRoi;
	_listOfRegion = new Region[numOfRoi];
}

void ROI::ReadFile(string fileName)
{
	ifstream fin(fileName, ios::in);
	string data;
	getline(fin,data); //ENVI Output of ROIs...
	getline(fin,data); //Number of ROIS

	//ROI个数
	int numOfROI= data[data.size() - 1]-'0';
	_numOfROI = numOfROI;
    if(_listOfRegion != nullptr) delete _listOfRegion;
	_listOfRegion = new Region[numOfROI];

	//暂存每个ROI的点数
	int* tempStoreNum = new int[numOfROI];

	getline(fin,data); //File Dimension 
	for (int i = 0; i < numOfROI; i++)
	{
		getline(fin, data);		//空
		//设置ROI Name
        getline(fin, data);		//ROI name
        int head = data.find(':');
        string name = data.substr(head + 1);
        Trim(name);
        _listOfRegion[i].SetName(name);
		//设置RGB Value
		getline(fin, data);		//ROI rgb value
        int loc = data.find(':');
        string rgb = data.substr(loc + 1);
        Trim(rgb);
        Trim(rgb, '{');
        Trim(rgb, '}');
        _listOfRegion[i].SetRGBValue(rgb);
		//设置point num
		getline(fin, data);		//ROI npts
        loc = data.find(':');
        int numOfPoint=atoi(data.substr(loc + 1).c_str());
		tempStoreNum[i]=numOfPoint;
	}
	getline(fin, data); //ID X Y...
    Trim(data, ';');
    vector<string> splitString;
    Split(splitString, data);
    int bandNum = splitString.size();
	for (int i = 0; i < numOfROI; i++)
	{
		_listOfRegion[i].SetNumOfPointAndBand(tempStoreNum[i], bandNum);
		//读取每个Region的数据
		_listOfRegion[i].ReadFromFile(fin);
		getline(fin, data);		//空
	}
	fin.close();
    delete[] tempStoreNum;
}

void ROI::ComputeAverage(vector<vector<double> > &averBands)
{
    for(int i = 0; i < _numOfROI; i++)
    {
        averBands.push_back(vector<double>());
        _listOfRegion[i].PrintAverage(averBands[i]);
    }
}

void ROI::SetClassHeader(Clas_Header &header)
{
    header.classes = _numOfROI + 1;

    header.lookUp = new Color[_numOfROI + 1];
    for(int i = 0; i < _numOfROI; i++)
    {
        header.lookUp[i + 1].r = _listOfRegion[i].rgbValue.r;
        header.lookUp[i + 1].g = _listOfRegion[i].rgbValue.g;
        header.lookUp[i + 1].b = _listOfRegion[i].rgbValue.b;
    }
    header.lookUp[0].r = 0; header.lookUp[0].g = 0; header.lookUp[0].b = 0;

    header.names = new string[_numOfROI + 1];
    for(int i = 0; i < _numOfROI; i++)
    {
        header.names[i + 1] = _listOfRegion[i].name;
    }
    header.names[0] = "Unclassified";
}
