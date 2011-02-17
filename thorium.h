/**********************************************************\

  Auto-generated thorium.cpp

  This file contains the auto-generated main plugin object
  implementation for the thorium project

\**********************************************************/
#ifndef H_thoriumPLUGIN
#define H_thoriumPLUGIN

#include "PluginWindow.h"
#include "PluginEvents/MouseEvents.h"
#include "PluginEvents/AttachedEvent.h"

#include "PluginCore.h"

class thorium : public FB::PluginCore
{
public:
    static void StaticInitialize();
    static void StaticDeinitialize();

public:
    thorium();
    virtual ~thorium();

public:
    void onPluginReady();
    virtual FB::JSAPIPtr createJSAPI();

    BEGIN_PLUGIN_EVENT_MAP()
    END_PLUGIN_EVENT_MAP()
};
typedef boost::shared_ptr<thorium> thoriumPtr;
typedef boost::weak_ptr<thorium> thoriumWeakPtr;


#endif

