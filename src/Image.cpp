#include "headers/Image.h"
#include <QImage>
#include <QPoint>

Image::Image()
{
    m_data = nullptr;
    m_bandMax = nullptr;
    m_bandMin = nullptr;
    m_id = ++num;
}

Image::Image(string & header, string & fileData)
{
    m_id = ++num;
    ReaderHeader(header);
    Initialized();
    ReadBinaryData(fileData);
}

Image::Image(Mat img)
{
    m_id = ++num;
    m_headInfo.m_lines = img.rows;
    m_headInfo.m_samples = img.cols;
    m_headInfo.m_bands = img.channels();
    m_headInfo.m_dataType = DATATYPE::UNSIGNED_INTERGER_16;
    m_headInfo.m_fileType = "ENVI Standard";
    m_headInfo.m_byteOrder = 0;
    m_headInfo.m_interleave = INTERLEAVE::BSQ;
    m_headInfo.m_sensorType = "Unknown";
    m_headInfo.m_description = "??ENVI_APP??????????????????";
    m_headInfo.m_headerOffset = 0;
    Initialized();

    int rows = m_headInfo.m_lines;
    int cols = m_headInfo.m_samples;
    for(int row = 0; row < rows; row++)
    {
        uchar* ptr = img.data + row * img.step;
        for(int col = 0; col < cols; col++)
        {
            for(int band = 0; band < m_headInfo.m_bands; band++)
            {
                m_data[band][row * cols + col] = ptr[band];
            }
            ptr += img.channels();
        }
    }

    for(int band = 0; band < m_headInfo.m_bands; band++)
    {
        m_bandMax[band] = 255;
        m_bandMin[band] = 0;
    }
}

Image::~Image()
{
    for (int band = 0; band < m_headInfo.m_bands; band++)
    {
        delete[] m_data[band];
    }
    delete[] m_data;
    delete[] m_bandMax;
    delete[] m_bandMin;
}

void Image::ShowImage(QPainter *painter, int bandNum) const
{
    //???????????
    QImage image(m_headInfo.m_samples, m_headInfo.m_lines, QImage::Format_Indexed8);
    //????????????
    QVector<QRgb> colorMap;
    for(int i = 0; i < 256; i++)
    {
        colorMap.push_back(qRgb(i, i, i));
    }
    image.setColorTable(colorMap);

    double maxValue = m_bandMax[bandNum];
    double minValue = m_bandMin[bandNum];
    int rows = m_headInfo.m_lines;
    int cols = m_headInfo.m_samples;
    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            uint pixelValue = (m_data[bandNum][row * cols + col] - minValue) / (maxValue - minValue) * 255;
            // ????????
            image.setPixel(col, row,  pixelValue);
        }
    }
    //??QWidget???
    painter->drawImage(QPoint(0, 0), image);
}

void Image::SetBandName(string bandName)
{
    if(m_headInfo.m_bandNames.size() != 0)
        m_headInfo.m_bandNames.clear();
    m_headInfo.m_bandNames.push_back(bandName);
}

void Image::SetFileName(string fileName)
{
    m_fileName = fileName;
}

int Image::GetId() const
{
    return m_id;
}

Mat Image::ConvertToOpencvMat(int band) const
{
    //创建Mat对象
    Mat img(m_headInfo.m_lines, m_headInfo.m_samples, CV_8UC1);
    int rows = m_headInfo.m_lines;
    int cols = m_headInfo.m_samples;
    //循环读取数据
    for(int row = 0; row < rows; row++)
    {
        uchar* ptr = img.ptr<uchar>(row);
        for(int col = 0; col < cols; col++)
        {
            uchar pixel = (m_data[band][row * cols + col] - m_bandMin[band]) / (m_bandMax[band] - m_bandMin[band]) * 255;
            ptr[col] = pixel;
        }
    }
    //返回
    return img;
}

Mat Image::ConvertToOpencvMat() const
{
    if(m_headInfo.m_bands != 3)
        return Mat();
    //创建Mat对象
    Mat img(m_headInfo.m_lines, m_headInfo.m_samples, CV_8UC3);
    int rows = m_headInfo.m_lines;
    int cols = m_headInfo.m_samples;
    //循环读取数据
    for(int row = 0; row < rows; row++)
    {
        Vec3b* ptr = img.ptr<Vec3b>(row);
        for(int col = 0; col < cols; col++)
        {
            for(int band = 0; band < 3; band++)
            {
                uchar pixel = (m_data[band][row * cols + col] - m_bandMin[band]) / (m_bandMax[band] - m_bandMin[band]) * 255;
                (*ptr)[band] = pixel;
            }
            ptr++;
        }
    }
    //返回
    return img;
}

void Image::ShowImage(QPainter *painter, int* bands) const
{
    if(bands[0] == bands[1] && bands[1] == bands[2])
        ShowImage(painter, bands[0]);    //???????????????????????????
    else
    {
        //????RGB32?????
        QImage image(m_headInfo.m_samples, m_headInfo.m_lines, QImage::Format_RGB32);
        int rows = m_headInfo.m_lines;
        int cols = m_headInfo.m_samples;
        for(int row = 0; row < rows; row++)
        {
            for(int col = 0; col < cols; col++)
            {
                int r = (m_data[bands[0]][row * cols + col] - m_bandMin[bands[0]]) / (m_bandMax[bands[0]] - m_bandMin[bands[0]]) * 255;
                int g = (m_data[bands[1]][row * cols + col] - m_bandMin[bands[1]]) / (m_bandMax[bands[1]] - m_bandMin[bands[1]]) * 255;
                int b = (m_data[bands[2]][row * cols + col] - m_bandMin[bands[2]]) / (m_bandMax[bands[2]] - m_bandMin[bands[2]]) * 255;
                QRgb value = qRgb(r, g, b);
                //???????????
                image.setPixel(col, row, value);
            }
        }
        //?????QWidget??
        painter->drawImage(QPoint(0, 0), image);
    }
}

void Image::Save(string filePath)
{
    //????????
    int index = filePath.find_last_of('.');
    if(index < 0)
        SaveHeader(filePath +".hdr");
    else
        SaveHeader(filePath.substr(0, index) + ".hdr");
    SaveData(filePath);
}

const HeadInfo &Image::GetHeaderInfo() const
{
    return m_headInfo;
}

void Image::ReaderHeader(string & header)
{
    ifstream fin_hdr(header, ios::in);
    string data;
    getline(fin_hdr, data);
    // ?????ж??????????ENVI??????????????????????
    if (data != "ENVI") {}
    //?????????
    while (getline(fin_hdr, data))
    {
        string keyInfo = data.substr(0, data.find('='));
        Trim(keyInfo);
        //description??
        if (keyInfo == "description")
        {
            do
            {
                getline(fin_hdr, data);
                m_headInfo.m_description += data;
            } while (data[data.size() - 1] != '}');
            Trim(m_headInfo.m_description, '}');
        }
        //samples??
        if (keyInfo == "samples")
        {
            int samples = atoi(data.substr(data.find('=') + 1).c_str());
            m_headInfo.m_samples = samples;
        }
        //lines??
        if (keyInfo == "lines")
        {
            int lines = atoi(data.substr(data.find('=') + 1).c_str());
            m_headInfo.m_lines = lines;
        }
        //bands??
        if (keyInfo == "bands")
        {
            int bands = atoi(data.substr(data.find('=') + 1).c_str());
            m_headInfo.m_bands = bands;
        }
        //head offset??
        if (keyInfo == "header offset")
        {
            int offset = atoi(data.substr(data.find('=') + 1).c_str());
            m_headInfo.m_headerOffset = offset;
        }
        //file type??
        if (keyInfo == "file type")
        {
            string fileType = data.substr(data.find('=') + 1);
            Trim(fileType);
            m_headInfo.m_fileType = fileType;
        }
        //data type??
        if (keyInfo == "data type")
        {
            int offset = atoi(data.substr(data.find('=') + 1).c_str());
            m_headInfo.m_dataType = DATATYPE(offset);
        }
        //interleave??
        if (keyInfo == "interleave")
        {
            string interLeave = data.substr(data.find('=') + 1);
            Trim(interLeave);
            if (interLeave == "bsq")
                m_headInfo.m_interleave = INTERLEAVE::BSQ;
            if (interLeave == "bip")
                m_headInfo.m_interleave = INTERLEAVE::BIP;
            if (interLeave == "bil")
                m_headInfo.m_interleave = INTERLEAVE::BIL;
        }
        //sensor type??
        if (keyInfo == "sensor type")
        {
            m_headInfo.m_sensorType = data.substr(data.find('=') + 1);
        }
        //byte order??
        if (keyInfo == "byte order")
        {
            int order = atoi(data.substr(data.find('=') + 1).c_str());
            m_headInfo.m_byteOrder = order;
        }
        //byte order??
        if (keyInfo == "band names")
        {
            string temp;
            do
            {
                getline(fin_hdr, data);
                temp += data;
            } while (data[data.size() - 1] != '}');
            Trim(temp, '}');
            Split(m_headInfo.m_bandNames, temp, ',');
        }
    }
    fin_hdr.close();
}

void Image::ReadBinaryData(string & fileName)
{
    m_fileName =fileName.substr(fileName.find_last_of('/')+1);
    ifstream fin(fileName, ios::in | ios::binary);
    //????header_offset?????
    char temp[128];
    fin.read(temp, m_headInfo.m_headerOffset);

    int rows = m_headInfo.m_lines;
    int cols = m_headInfo.m_samples;

    char* buf = new char[m_bytesPerPixel];
    //??????BSQ??
    if (m_headInfo.m_interleave == INTERLEAVE::BSQ)
    {
        for (int i = 0; i < m_headInfo.m_bands; i++)
        {
            double max_value, min_value;
            for (int row = 0; row < rows; row++)
            {
                for (int col = 0; col < cols; col++)
                {
                    fin.read(buf, m_bytesPerPixel);
                    m_data[i][row*cols + col] = Byte2Double(buf, m_headInfo.m_dataType);
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
                    fin.read(buf, m_bytesPerPixel);
                    m_data[k][i*cols + j] = Byte2Double(buf, m_headInfo.m_dataType);
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
    fin.close();
    delete[] buf;
}

void Image::Initialized()
{
    m_data = new double*[m_headInfo.m_bands];
    int rows = m_headInfo.m_lines;
    int cols = m_headInfo.m_samples;
    for (int band = 0; band < m_headInfo.m_bands; band++)
    {
        m_data[band] = new double[rows*cols];
    }

    m_bandMax = new double[m_headInfo.m_bands];
    m_bandMin = new double[m_headInfo.m_bands];

    m_bytesPerPixel = CalculateByte(m_headInfo.m_dataType);
}

void Image::SaveHeader(const string & fileName)
{
    ofstream fout(fileName, ios::out);
    fout << "ENVI" << endl;
    fout << "description = {" << endl << m_headInfo.m_description << "}" << endl;
    fout << "samples = " << m_headInfo.m_samples << endl;
    fout << "lines = " << m_headInfo.m_lines << endl;
    fout << "bands = " << m_headInfo.m_bands << endl;
    fout << "header offset = " << m_headInfo.m_headerOffset << endl;
    fout << "file type = " << m_headInfo.m_fileType << endl;
    fout << "data type = " << m_headInfo.m_dataType << endl;
    if(m_headInfo.m_interleave == INTERLEAVE::BSQ)
        fout << "interleave = " << "bsq" << endl;
    if(m_headInfo.m_interleave == INTERLEAVE::BIP)
        fout << "interleave = " << "bip" << endl;
    if(m_headInfo.m_interleave == INTERLEAVE::BIL)
         fout << "interleave = " << "bil" << endl;
    fout << "sensor type = " << m_headInfo.m_sensorType << endl;
    fout << "byte order = " << m_headInfo.m_byteOrder << endl;
    fout << "band names = {" << endl;
    for(int i = 0; i < m_headInfo.m_bands; i++)
    {
        if(i != m_headInfo.m_bands-1)
            fout << m_headInfo.m_bandNames[i] << ","<<endl;
        else
            fout << m_headInfo.m_bandNames[i] << "}" << endl;
    }
    fout.close();
}

void Image::SaveData(const string & fileName)
{
    ofstream fout(fileName, ios::binary | ios::out);

    int rows = m_headInfo.m_lines;
    int cols = m_headInfo.m_samples;
    int bands = m_headInfo.m_bands;
    char* buf = new char[m_bytesPerPixel];

    if (m_headInfo.m_interleave == BSQ)
    {
        for (int band = 0; band < bands; band++)
        {
            for (int row = 0; row < rows; row++)
            {
                for (int col = 0; col < cols; col++)
                {
                    Double2Bytes(buf, m_data[band][row*cols + col], m_headInfo.m_dataType);
                    fout.write(buf, m_bytesPerPixel);
                }
            }
        }
    }
    if (m_headInfo.m_interleave == BIP)
    {
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                for (int band = 0; band < bands; band++)
                {
                    Double2Bytes(buf, m_data[band][row*cols + col], m_headInfo.m_dataType);
                    fout.write(buf, m_bytesPerPixel);
                }
            }
        }
    }
    fout.close();
    delete[] buf;
}

string Image::GetFileName() const
{
    return m_fileName;
}

void Image::StructureData(vector<vector<double> > &structuredData)
{
    int rows = m_headInfo.m_lines;
    int cols = m_headInfo.m_samples;
    int bands = m_headInfo.m_bands;
    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            structuredData.push_back(vector<double>(bands));
            for(int band = 0; band < bands; band++)
            {
                structuredData[row * cols + col][band] = m_data[band][row * cols + col];
            }
        }
    }
}

vector<double> Image::operator[](int n) const
{
    int bands = m_headInfo.m_bands;
    vector<double> bandFeatures(bands);
    for(int band = 0; band < bands; band++)
    {
        bandFeatures[band] = m_data[band][n];
    }
    return bandFeatures;
}

Class_Image::Class_Image(uchar *data, int rows, int cols, ROI* roi)
{
    m_headInfo.m_lines = rows;
    m_headInfo.m_samples = cols;
    m_headInfo.m_bands = 1;
    m_headInfo.m_dataType = DATATYPE::BYTE_8;
    m_headInfo.m_fileType = "ENVI Classification";
    m_headInfo.m_byteOrder = 0;
    m_headInfo.m_interleave = INTERLEAVE::BSQ;
    m_headInfo.m_sensorType = "Unknown";
    m_headInfo.m_description = "??ENVI_APP??????????????????";
    m_headInfo.m_headerOffset = 0;
    Initialized();

    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            m_data[0][row * cols + col] = data[row * cols + col];
        }
    }

    SetClasHeader(roi);
}

Class_Image::Class_Image(string & header, string & fileData)
    :Image(header, fileData)
{
    ReadClasHeader(header);
}

Class_Image::~Class_Image()
{
    delete[] m_clasHeader.lookUp;
    delete[] m_clasHeader.names;
}

void Class_Image::ShowImage(QPainter *painter, int bandNum) const
{
    //???????????
    QImage image(m_headInfo.m_samples, m_headInfo.m_lines, QImage::Format_Indexed8);
    //????????????
    QVector<QRgb> colorMap;
    for(int i = 0; i < m_clasHeader.classes; i++)
    {
        colorMap.push_back(qRgb(m_clasHeader.lookUp[i].r, m_clasHeader.lookUp[i].g, m_clasHeader.lookUp[i].b));
    }
    image.setColorTable(colorMap);

    int rows = m_headInfo.m_lines;
    int cols = m_headInfo.m_samples;
    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            image.setPixel(col, row,  (uint)m_data[bandNum][row * cols + col]);
        }
    }
    //??QWidget???
    painter->drawImage(QPoint(0, 0), image);
}

void Class_Image::SetClasHeader(ROI *roi)
{
    roi->SetClassHeader(m_clasHeader);
}

void Class_Image::ShowImage(QPainter *painter, int* bands) const
{
    //?????????????????
    ShowImage(painter, bands[0]);
}

int Class_Image::GetClasses() const
{
    return m_clasHeader.classes;
}

const Clas_Header &Class_Image::GetClasHeader() const
{
    return m_clasHeader;
}

void Class_Image::ReadClasHeader(string & fileName)
{
    ifstream fin(fileName, ios::in);
    string str;
    while (getline(fin, str))
    {
        int index = str.find('=');
        string keyInfo = str.substr(0, index);    //key?
        Trim(keyInfo);    //??????
        if (keyInfo == "classes")
        {
            //?????int
            int classes = atoi(str.substr(index + 1).c_str());
            m_clasHeader.classes = classes;
        }
        if (keyInfo=="class names")
        {
            string temp;
            do
            {
               getline(fin, str);
               temp += str;
            } while(str[str.size() - 1] != '}');
            Trim(temp, '}');
            vector<string> splitString;
            //??????????????
            Split(splitString, temp, ',');
            int classes = splitString.size();
            //???????
            m_clasHeader.names = new string[classes];
            for (int i = 0; i < classes; i++)
            {
                m_clasHeader.names[i] = splitString[i];
            }
        }
        if (keyInfo=="class lookup")
        {
            string temp;
            do
            {
               getline(fin, str);
               temp += str;
            } while(str[str.size() - 1] != '}');
            Trim(temp, '}');
            vector<string> splitString;
            //??????????????
            Split(splitString, temp, ',');
            //???????
            m_clasHeader.lookUp = new Color[m_clasHeader.classes];
            for (int i = 0; i < m_clasHeader.classes; i++)
            {
                m_clasHeader.lookUp[i].r = atoi(splitString[3 * i + 0].c_str());
                m_clasHeader.lookUp[i].g = atoi(splitString[3 * i + 1].c_str());
                m_clasHeader.lookUp[i].b = atoi(splitString[3 * i + 2].c_str());
            }
        }
    }
    fin.close();
}

void Class_Image::SaveHeader(const string & fileName)
{
    Image::SaveHeader(fileName);
    ofstream fout(fileName, ios::out|ios::app);
    //??????
    fout << "classes = " << m_clasHeader.classes << endl;
    //????????
    fout << "class lookup = {" << endl;
    for (int i = 0; i < m_clasHeader.classes; i++)
    {
        fout << m_clasHeader.lookUp[i].r << ", ";
        fout << m_clasHeader.lookUp[i].g << ", ";
        if (i == m_clasHeader.classes - 1)
            fout << m_clasHeader.lookUp[i].b << "}";
        else
            fout << m_clasHeader.lookUp[i].b << ", ";
    }
    //????????
    fout << endl << "class names = {" << endl;
    for (int i = 0; i < m_clasHeader.classes; i++)
    {
        if (i == m_clasHeader.classes - 1)
            fout << m_clasHeader.names[i] << "}";
        else
            fout << m_clasHeader.names[i] << ", ";
    }
    fout.close();
}

int Image::num = 0;
