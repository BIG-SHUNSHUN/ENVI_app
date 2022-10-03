#include "ENVI.h"

void Trim(string & s, char ch)
{
	int low = s.find_first_not_of(ch);
	if (low != 0)
	{
		s.erase(0, low);
	}
	int high = s.find_last_not_of(ch);
	if (high != s.size() - 1)
	{
		s.erase(high + 1);
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

void Split(vector<string>& splitString, string&s, char ch)
{
	//��string���stringstream
	stringstream sstream(s);
	string temp;
	while (getline(sstream, temp, ch))
	{
		Trim(temp);    //ɾ����β�Ŀո�
		Trim(temp, '}');    //ɾ����β�� }
		splitString.push_back(temp);
	}
}

Image::Image(string & header, string & fileData)
{
	ReaderHeader(header);
	Initialized();
	ReadBinaryData(fileData);
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

void Image::ShowImage(int bandNum)
{
	int rows = m_headInfo.m_lines;
	int cols = m_headInfo.m_samples;
	Mat mat(rows, cols, CV_8UC1);
	//�ҵ�������Сֵ
	double max_value = m_bandMax[bandNum], min_value = m_bandMin[bandNum];
	//���쵽0-255
	for (int i = 0; i < rows; i++)
	{
		uchar * ptr = mat.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			ptr[j] = (m_data[bandNum][i*cols + j] - min_value) / (max_value - min_value) * 255;
		}
	}
	imshow("��ʾ", mat);
	waitKey(0);
}

void Image::Save(string & filePath)
{
	SaveHeader(filePath + "\\save.hdr");
	SaveData(filePath + "\\save.dat");
}

void Image::ReaderHeader(string & header)
{
	ifstream fin_hdr(header, ios::in);
	string data;
	getline(fin_hdr, data);
	// �����жϵ�һ���ǲ���ENVI�����Ǿ��˳������������
	if (data != "ENVI") {}
	//ѭ����ȡ��Ϣ
	while (getline(fin_hdr, data))
	{
		string keyInfo = data.substr(0, data.find('='));
		Trim(keyInfo);
		//description��
		if (keyInfo == "description")
		{
			do
			{
				getline(fin_hdr, data);
				m_headInfo.m_description += data;
			} while (data[data.size() - 1] != '}');
			Trim(m_headInfo.m_description, '}');
		}
		//samples��
		if (keyInfo == "samples")
		{
			int samples = atoi(data.substr(data.find('=') + 1).c_str());
			m_headInfo.m_samples = samples;
		}
		//lines��
		if (keyInfo == "lines")
		{
			int lines = atoi(data.substr(data.find('=') + 1).c_str());
			m_headInfo.m_lines = lines;
		}
		//bands��
		if (keyInfo == "bands")
		{
			int bands = atoi(data.substr(data.find('=') + 1).c_str());
			m_headInfo.m_bands = bands;
		}
		//head offset��
		if (keyInfo == "header offset")
		{
			int offset = atoi(data.substr(data.find('=') + 1).c_str());
			m_headInfo.m_headerOffset = offset;
		}
		//file type��
		if (keyInfo == "file type")
		{
			m_headInfo.m_fileType = data.substr(data.find('=') + 1);
		}
		//data type��
		if (keyInfo == "data type")
		{
			int offset = atoi(data.substr(data.find('=') + 1).c_str());
			m_headInfo.m_dataType = DATATYPE(offset);
		}
		//interleave��
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
		//sensor type��
		if (keyInfo == "sensor type")
		{
			m_headInfo.m_sensorType = data.substr(data.find('=') + 1);
		}
		//byte order��
		if (keyInfo == "byte order")
		{
			int order = atoi(data.substr(data.find('=') + 1).c_str());
			m_headInfo.m_byteOrder = order;
		}
	}
	fin_hdr.close();
}

void Image::ReadBinaryData(string & fileName)
{
	ifstream fin(fileName, ios::in | ios::binary);
	//�ȶ�ȡheader_offset���ֽ�
	char temp[128];
	fin.read(temp, m_headInfo.m_headerOffset);

	int rows = m_headInfo.m_lines;
	int cols = m_headInfo.m_samples;

	char* buf = new char[m_bytesPerPixel];
	//�����BSQ�洢
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
	fout << "interleave = " << m_headInfo.m_interleave << endl;
	fout << "sensor type = " << m_headInfo.m_sensorType << endl;
	fout << "byte order = " << m_headInfo.m_byteOrder << endl;
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

void Class_Image::ShowImage(int bandNum)
{
	if (bandNum>m_headInfo.m_bands-1)
	{
		return;
	}
	int rows = m_headInfo.m_lines;
	int cols = m_headInfo.m_samples;
	Mat mat(rows, cols, CV_8UC3);
	//����color lookup�ֶ���ʾ
	for (int i = 0; i < rows; i++)
	{
		Vec3b * ptr = mat.ptr<Vec3b>(i);
		for (int j = 0; j < cols; j++)
		{
			unsigned char value = m_data[bandNum][i*cols + j];
			ptr[j][0] = m_clasHeader.lookUp[value].b;
			ptr[j][1] = m_clasHeader.lookUp[value].g;
			ptr[j][2] = m_clasHeader.lookUp[value].r;
		}
	}
	imshow("��ʾ", mat);
	waitKey(0);
}

void Class_Image::ReadClasHeader(string & fileName)
{
	ifstream fin(fileName, ios::in);
	string str;
	while (getline(fin, str))
	{
		int index = str.find('=');
		string keyInfo = str.substr(0, index);    //keyֵ
		Trim(keyInfo);    //ɾ���ո�
		if (keyInfo == "classes")
		{
			//ת����int
			int classes = atoi(str.substr(index + 1).c_str());
			m_clasHeader.classes = classes;
		}
		if (keyInfo=="class names")
		{
			getline(fin, str);
			vector<string> splitString;
			//���ݶ��ŷָ��ַ���
			Split(splitString, str, ',');
			int classes = splitString.size();
			//����ռ�
			m_clasHeader.names = new string[classes];
			for (int i = 0; i < classes; i++)
			{
				m_clasHeader.names[i] = splitString[i];
			}
		}
		if (keyInfo=="class lookup")
		{
			getline(fin, str);
			vector<string> splitString;
			//���ݶ��ŷָ��ַ���
			Split(splitString, str, ',');
			//����ռ�
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
	fout << "classes = " << m_clasHeader.classes << endl;
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
