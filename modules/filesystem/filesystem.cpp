/**********************************************************\
Copyright Mital Vora <mital.d.vora@gmail.com> 
\**********************************************************/


#include "filesystem.h"

#include "file.h"


namespace th
{

	Filesystem::Filesystem()
	{
		registerMethod("getFile", make_method(this, &Filesystem::getFile));
	}

	
	FB::JSAPIPtr Filesystem::getFile(const std::string & filename)
	{
		return boost::shared_ptr<File>(new File(filename));
	}
}