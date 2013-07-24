#include "ConfigLoader.h"


#include <fstream>


#define GOOD_CHARS "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_-1234567890.:/\\"

int findOneOf(const char* _in, const char* _chars)
{
	for(int i = 0; i < strlen( _in); ++i)
	{
		for(int j = 0; j < strlen(_chars); ++j)
		{
			if(_in[i] == _chars[j])
				return i;
		}
	}


	return -1;
}


int findOneNotOf(const char* _in, const char* _chars)
{

	bool found = false;
	for(int i = 0; i < strlen(_in); ++i)
	{
		found = false;
		for(int j = 0; j < strlen(_chars); ++j)
		{
			if(_in[i] == _chars[j])
			{
				found = true;
				break;
			}
		}

		if(!found)
			return i;
	}

	return strlen(_in);
}

ConfigLoader::ConfigLoader()
{

}


ConfigLoader::~ConfigLoader()
{
	
}


bool ConfigLoader::parseFile(const std::string& _path)
{
	std::ifstream file;

	file.open(_path.c_str(), std::ios::in | std::ios::binary);

	if(!file.is_open())
		return false;

	file.seekg(0, std::ios::end);
	int size = file.tellg();
	file.seekg(0, std::ios::beg);


	char* data = new char[size+1];
	file.read(data, size);
	data[size] = '\0';

	
	bool ret = parseMemory(data, size);

	delete data;

	return ret;
}

bool ConfigLoader::parseMemory(char *_memory, int _size)
{
	int readed = 0;
	int positionStart = 0;
	int positionEnd = 0;
	
	int position = 0;

	std::string field("");
	std::string value("");
	while( position < _size)
	{


		positionStart = findOneOf(_memory + position, "#");

		if(positionStart != -1)
		{
			position += positionStart;
			positionEnd = findOneOf(_memory + position, "\n\r");

			if(positionEnd == -1)
				position = _size; // cala reszta pliku to komentarz, wiec konczymy
			else
				position += positionEnd; // zmieniamy pozycje czytania za komentarz
				

			continue;
		}

		positionStart = findOneOf(_memory + position, GOOD_CHARS);


		if(positionStart == -1)
			return false;

		position += positionStart;

		positionEnd = findOneNotOf(_memory + position, GOOD_CHARS);


		if(positionEnd == -1)
			return false;


		if(field.empty())
		{
			field.assign( _memory + position, positionEnd );
		}
		else if(value.empty())
		{
			value.assign( _memory + position, positionEnd );		
			mFieldMap.insert( std::pair<std::string, std::string>(field, value));
			field.clear();
			value.clear();
		}

		position += positionEnd;



		
	}

	return true;
}


bool ConfigLoader::getBoolValue(const std::string& _fieldName)
{
	FieldMap::iterator iter = mFieldMap.find(_fieldName);

	if(iter == mFieldMap.end())
		return true;

	if(iter->second == "false" || iter->second == "0" || iter->second == "FALSE")
		return false;

	return true;

}
int	ConfigLoader::getIntValue(const std::string& _fieldName)
{
	FieldMap::iterator iter = mFieldMap.find(_fieldName);

	if(iter == mFieldMap.end())
		return -1;

	return atoi(iter->second.c_str());

}

float ConfigLoader::getFloatValue(const std::string& _fieldName)
{
	FieldMap::iterator iter = mFieldMap.find(_fieldName);

	if(iter == mFieldMap.end())
		return -1;

	return (float)atof(iter->second.c_str());
}

std::string ConfigLoader::getStringValue(const std::string& _fieldName)
{
	FieldMap::iterator iter = mFieldMap.find(_fieldName);

	if(iter == mFieldMap.end())
		return "";

	return iter->second;

}



char*	ConfigLoader::dumpToMemory(int& _size)
{
	char* data;

	std::string configText;

	for(FieldMap::iterator iter = mFieldMap.begin(); iter != mFieldMap.end(); ++iter)
	{
		configText += iter->first;
		configText += " = ";
		configText += iter->second;
		configText += "\n";
	}

	data = new char[configText.size()];
	memcpy((void*)data, (void*)&configText[0], configText.size());
	_size = configText.size();
	return data;
}

bool	ConfigLoader::dumpToFile(const std::string& _path)
{

	std::ofstream file;

	file.open(_path.c_str(), std::ios_base::out | std::ios_base::trunc );

	if(!file.is_open())
		return false;

	int size = 0;
	char* data = dumpToMemory(size);

	file.write(data, size);
		

	return true;
	
	
}


void	ConfigLoader::setValue(const std::string& _fieldName, const std::string& _value)
{
	FieldMap::iterator iter = mFieldMap.find(_fieldName);

	if(iter == mFieldMap.end()) // jesli nie ma takiej
	{	
		addValue(_fieldName, _value); // dodajemy nowa
		return;
	}


	iter->second = _value;
}


bool ConfigLoader::addValue(const std::string& _fieldName, const std::string& _value)
{
	FieldMap::iterator iter = mFieldMap.find(_fieldName);

	if(iter == mFieldMap.end())
	{
		mFieldMap.insert( std::pair<std::string, std::string>(_fieldName, _value));
		return true;
	}

	return false;
}



