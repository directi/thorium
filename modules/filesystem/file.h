/**********************************************************\
Copyright Mital Vora <mital.d.vora@gmail.com> 
\**********************************************************/

#ifndef TH_FILE_H 
#define TH_FILE_H

#include <JSAPIAuto.h>

namespace th
{

///<summary>
/// This class gives access to a File.
///</summary>

class File : public FB::JSAPIAuto
{
	const std::string filename;
public:

	File(const std::string & filename);
	virtual ~File();

	std::string getFileName();
};

}
#endif // TH_FILESYSTEM_H
