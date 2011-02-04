/**********************************************************\

  Auto-generated thoriumAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "thorium.h"

#ifndef H_thoriumAPI
#define H_thoriumAPI

class thoriumAPI : public FB::JSAPIAuto
{
public:
    thoriumAPI(thoriumPtr plugin, FB::BrowserHostPtr host);
    virtual ~thoriumAPI();

    thoriumPtr getPlugin();

    // Expose a stub cursor for access to constants
    FB::JSAPIPtr getFilesystem();
    

private:
    thoriumWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    std::string m_testString;
};

#endif // H_thoriumAPI

