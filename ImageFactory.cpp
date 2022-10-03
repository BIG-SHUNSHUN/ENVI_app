#include "ImageFactory.h"

ImageFactory::ImageFactory()
{

}

Image *ImageFactory::CreateImage(string file)
{
    // 获得头文件路径
    int index = file.find_last_of('.');
    string fileHeader;
    if(index == -1)
        fileHeader = file + ".hdr";
    else
        fileHeader = file.substr(0,index) + ".hdr";
    IMAGETYPE type =  DecideImageType(fileHeader);
    switch (type)
    {
    case ENVI_STANDARD:
        return new Image(fileHeader, file);
        break;
    case ENVI_CLASSIFICATION:
        return new Class_Image(fileHeader, file);
        break;
    }
}

IMAGETYPE ImageFactory::DecideImageType(string &fileName)
{
    ifstream fin_hdr(fileName, ios::in);
    string data;
    getline(fin_hdr, data);
    // 首先判断第一行是不是ENVI，不是就退出，否则继续读
    if (data != "ENVI") {}
    //循环读取信息
    while (getline(fin_hdr, data))
    {
        string keyInfo = data.substr(0, data.find('='));
        Trim(keyInfo);
        if (keyInfo == "file type")
        {
            string typeInfo =data.substr(data.find('=') + 1);
            Trim(typeInfo);
            if(typeInfo == "ENVI Standard")
            {
                fin_hdr.close();
                return ENVI_STANDARD;
            }
            else if(typeInfo == "ENVI Classification")
            {
                fin_hdr.close();
                return ENVI_CLASSIFICATION;
            }
        }
    }
}
