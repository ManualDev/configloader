#ifndef _CONFIGLOADER_H
#define _CONFIGLOADER_H



#include <string>
#include <map>





typedef std::map<std::string, std::string> FieldMap;

/* Klasa do ³adowania tekstowych plików konfiguracyjnych:
	- £adowanie z pliku
	- £adowanie z pamiêci(w przypadku systemów VFS etc.)
	- Zmiana wartosci istniejacych pol
	- Dodawanie pol
	- Zapis do pliku
	- Zapis do pamieci
*/
/*
	W wielu miejscach mozna bylo oszczedzic kodu poprzez uzycia funkcji z biblioteki standardowej, ale celem tego projektu by³o coœ innego ; )
*/
class ConfigLoader
{
public:
					ConfigLoader();
					~ConfigLoader();


					bool		parseFile(const std::string&	_path);
					bool		parseMemory(char* _memory, int _size);


				
					char*		dumpToMemory(int& _size);
					bool		dumpToFile(const std::string& _path);


					void		setValue(const std::string& _fieldName, const std::string& _value);
					/*
						Zwraca false jesli takie pole juz istnieje
					*/
					bool		addValue(const std::string& _fieldName, const std::string& _value); 
		

					bool		getBoolValue(const std::string& _fieldName);
					int			getIntValue(const std::string& _fieldName);
					float		getFloatValue(const std::string& _fieldName);
					std::string getStringValue(const std::string& _fieldName);

private:

			
		
					FieldMap	mFieldMap;


};



#endif