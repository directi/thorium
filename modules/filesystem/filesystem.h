/**********************************************************\
Copyright Mital Vora <mital.d.vora@gmail.com> 
\**********************************************************/

#ifndef TH_FILESYSTEM_H 
#define TH_FILESYSTEM_H

#include <JSAPIAuto.h>

namespace th
{

///<summary>
/// This class represents the Filesystem Interface in Thorium. 
///</summary>
class Filesystem : public FB::JSAPIAuto
{
	public:
		static const boost::shared_ptr<Filesystem> getInstance()
		{ return boost::shared_ptr<Filesystem>(new Filesystem()); }

		FB::JSAPIPtr getFile(const std::string & filename);

	protected:
		Filesystem();
};

}
#endif // TH_FILESYSTEM_H
