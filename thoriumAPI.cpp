/**********************************************************\

  Auto-generated thoriumAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"

#include "thoriumAPI.h"
#include "modules/filesystem/filesystem.h"

///////////////////////////////////////////////////////////////////////////////
/// @fn thoriumAPI::thoriumAPI(thoriumPtr plugin, FB::BrowserHostPtr host)
///
/// @brief  Constructor for your JSAPI object.  You should register your methods, properties, and events
///         that should be accessible to Javascript from here.
///
/// @see FB::JSAPIAuto::registerMethod
/// @see FB::JSAPIAuto::registerProperty
/// @see FB::JSAPIAuto::registerEvent
///////////////////////////////////////////////////////////////////////////////
thoriumAPI::thoriumAPI(thoriumPtr plugin, FB::BrowserHostPtr host) : m_plugin(plugin), m_host(host)
{
	registerProperty("filesystem", make_property(this, &thoriumAPI::getFilesystem));
}

///////////////////////////////////////////////////////////////////////////////
/// @fn thoriumAPI::~thoriumAPI()
///
/// @brief  Destructor.  Remember that this object will not be released until
///         the browser is done with it; this will almost definitely be after
///         the plugin is released.
///////////////////////////////////////////////////////////////////////////////
thoriumAPI::~thoriumAPI()
{
}

///////////////////////////////////////////////////////////////////////////////
/// @fn thoriumPtr thoriumAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
thoriumPtr thoriumAPI::getPlugin()
{
    thoriumPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}


FB::JSAPIPtr thoriumAPI::getFilesystem()
{
	return th::Filesystem::getInstance();
}
