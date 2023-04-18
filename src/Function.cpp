#include "headers/Function.h"

void Trim(string & s, char ch)
{
    int low = s.find_first_not_of(ch);
    if (low != 0)
    {
        s.erase(0, low);
    }
    int high = s.find_last_not_of(ch);
    if (high != s.size()-1)
    {
        s.erase(high + 1);
    }
}

void Split(vector<string>& splitString, string&s, char ch)
{
    //把string变成stringstream
    stringstream sstream(s);
    string temp;
    while (getline(sstream, temp, ch))
    {
        if(temp.size() == 0) continue;
        Trim(temp);    //删除首尾的空格
        Trim(temp, '}');    //删除首尾的 }
        splitString.push_back(temp);
    }
}

int CalculateByte(DATATYPE type)
{
    switch (type)
    {
    case BYTE_8: return 1;
        break;
    case SIGNED_INTERGER_16: return 2;
        break;
    case SIGNED_LONG_INTERGER_32:return 4;
        break;
    case FLOAT_32: return 4;
        break;
    case DOUBLE_64: return 8;
        break;
    case UNSIGNED_INTERGER_16: return 2;
        break;
    case UNSIGNED_LONG_INTERGER_32: return 4;
        break;
    case SIGNED_LONG_INTERGER_64: return 8;
        break;
    case UNSIGNED_LONG_INTERGER_64: return 8;
        break;
    }
}

double Byte2Double(const char * ptr, DATATYPE & type)
{
    switch (type)
    {
    case BYTE_8: return *(unsigned char*)ptr;
        break;
    case SIGNED_INTERGER_16: return *(short*)ptr;
        break;
    case SIGNED_LONG_INTERGER_32:return *(int*)ptr;
        break;
    case FLOAT_32: return *(float*)ptr;
        break;
    case DOUBLE_64: return *(double*)ptr;
        break;
    case UNSIGNED_INTERGER_16: return *(unsigned short*)ptr;
        break;
    case UNSIGNED_LONG_INTERGER_32: return *(unsigned int*)ptr;
        break;
    case SIGNED_LONG_INTERGER_64: return *(long int *)ptr;
        break;
    case UNSIGNED_LONG_INTERGER_64: return *(unsigned long int *)ptr;
        break;
    }
}

void Double2Bytes(char* ptr,double& value,  DATATYPE & type)
{
    if (type==BYTE_8)
    {
        unsigned char temp = (unsigned char)value;
        *(unsigned char*)ptr = temp;
    }
    if (type == DATATYPE::DOUBLE_64)
    {
        *(double*)ptr = value;
    }
    if (type == DATATYPE::FLOAT_32)
    {
        float temp = (float)value;
        *(float*)ptr = temp;
    }
    if (type == DATATYPE::SIGNED_INTERGER_16)
    {
        short temp = (short)value;
        *(short*)ptr = temp;
    }
    if (type == DATATYPE::SIGNED_LONG_INTERGER_32)
    {
        int temp = (int)value;
        *(int*)ptr = temp;
    }
    if (type == DATATYPE::SIGNED_LONG_INTERGER_64)
    {
        long int temp = (long int)value;
        *(long int*)ptr = temp;
    }
    if (type == DATATYPE::UNSIGNED_INTERGER_16)
    {
        unsigned short temp = (unsigned short)value;
        *(unsigned short*)ptr = temp;
    }
    if (type == DATATYPE::UNSIGNED_LONG_INTERGER_32)
    {
        unsigned int temp = (unsigned int)value;
        *(unsigned int*)ptr = temp;
    }
    if (type == DATATYPE::UNSIGNED_LONG_INTERGER_64)
    {
        unsigned long int temp = (unsigned long int)value;
        *(unsigned long int*)ptr = temp;
    }
}

DATATYPE JudgeDatatype(Mat img)
{
    switch (img.type())
    {
    case CV_8UC1:
    case CV_8UC2:
    case CV_8UC3:
    case CV_8UC4:
        return DATATYPE::BYTE_8;
        break;
    case CV_8SC1:
    case CV_8SC2:
    case CV_8SC3:
    case CV_8SC4:
        return DATATYPE::SIGNED_INTERGER_16;
        break;
    case CV_16UC1:
    case CV_16UC2:
    case CV_16UC3:
    case CV_16UC4:
        return DATATYPE::UNSIGNED_INTERGER_16;
        break;
    case CV_16SC1:
    case CV_16SC2:
    case CV_16SC3:
    case CV_16SC4:
        return DATATYPE::SIGNED_INTERGER_16;
        break;
    case CV_16FC1:
    case CV_16FC2:
    case CV_16FC3:
    case CV_16FC4:
        return DATATYPE::FLOAT_32;
        break;
    case CV_32SC1:
    case CV_32SC2:
    case CV_32SC3:
    case CV_32SC4:
        return DATATYPE::SIGNED_LONG_INTERGER_32;
        break;
    case CV_32FC1:
    case CV_32FC2:
    case CV_32FC3:
    case CV_32FC4:
        return DATATYPE::FLOAT_32;
        break;
    case CV_64FC1:
    case CV_64FC2:
    case CV_64FC3:
    case CV_64FC4:
        return DATATYPE::DOUBLE_64;
        break;
    }
}
