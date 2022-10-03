void Trim(string & s)
{
	int low = s.find_first_not_of(' ');
	if (low!=0)
	{
		s.erase(0, low);
	}
	int high = s.find_last_not_of(' ');
	if (high!=s.size()-1)
	{
		s.erase(high + 1);
	}
}

HeadInfo ReadHeaderInfo(string & fileHeader)
{
	HeadInfo m_headInfo;
	ifstream fin_hdr(fileHeader, ios::in);
	string data;
	getline(fin_hdr, data);
	// 首先判断第一行是不是ENVI，不是就退出，否则继续读
	if (data != "ENVI")
	{
		return HeadInfo();
	}
	//循环读取信息
	while (getline(fin_hdr, data))
	{
		string keyInfo = data.substr(0, data.find('='));
		Trim(keyInfo);
		//description键
		if (keyInfo == "description")
		{
			do
			{
				getline(fin_hdr, data);
				m_headInfo.m_description += (data + '\n');
			} while (data[data.size() - 1] != '}');
			m_headInfo.m_description.pop_back();
		}
		//samples键
		if (keyInfo == "samples")
		{
			int samples = atoi(data.substr(data.find('=') + 1).c_str());
			m_headInfo.m_samples = samples;
		}
		//lines键
		if (keyInfo == "lines")
		{
			int lines = atoi(data.substr(data.find('=') + 1).c_str());
			m_headInfo.m_lines = lines;
		}
		//bands键
		if (keyInfo == "bands")
		{
			int bands = atoi(data.substr(data.find('=') + 1).c_str());
			m_headInfo.m_bands = bands;
		}
		//head offset键
		if (keyInfo == "header offset")
		{
			int offset = atoi(data.substr(data.find('=') + 1).c_str());
			m_headInfo.m_headerOffset = offset;
		}
		//file type键
		if (keyInfo == "file type")
		{
			m_headInfo.m_fileType = data.substr(data.find('=') + 1);
		}
		//data type键
		if (keyInfo == "data type")
		{
			int offset = atoi(data.substr(data.find('=') + 1).c_str());
			m_headInfo.m_dataType = DATATYPE(offset);
		}
		//interleave键
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
		//sensor type键
		if (keyInfo == "sensor type")
		{
			m_headInfo.m_sensorType = data.substr(data.find('=') + 1);
		}
		//byte order键
		if (keyInfo == "byte order")
		{
			int order = atoi(data.substr(data.find('=') + 1).c_str());
			m_headInfo.m_byteOrder = order;
		}
	}
	fin_hdr.close();
	return m_headInfo;
}