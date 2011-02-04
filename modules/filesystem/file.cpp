/**********************************************************\
Copyright Mital Vora <mital.d.vora@gmail.com> 
\**********************************************************/


#include "file.h"


namespace th
{

	File::File(const std::string & filename)
	: filename(filename)
	{
		registerMethod("getFileName", make_method(this, &File::getFileName));
	}
	
	File::~File()
	{
	}

	
	std::string File::getFileName()
	{
		return filename;
	}
}