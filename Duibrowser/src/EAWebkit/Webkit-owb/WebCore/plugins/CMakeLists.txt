LIST (APPEND PLUGINS_SRC
    plugins/PluginDatabase.cpp
    plugins/PluginInfoStore.cpp
    plugins/PluginPackage.cpp
    plugins/PluginStream.cpp

    plugins/PluginData.cpp
    plugins/PluginView.cpp
    plugins/MimeType.cpp
    plugins/PluginArray.cpp
    plugins/MimeTypeArray.cpp
    plugins/Plugin.cpp
    plugins/npapi.cpp
)

IF (WEBKIT_USE_NPAPI)
	IF (OWBAL_PLATFORM_GRAPHICS STREQUAL "GTK")
    	LIST (APPEND PLUGINS_SRC
	        plugins/gtk/PluginDatabaseGtk.cpp
	        plugins/gtk/PluginPackageGtk.cpp
    	    plugins/gtk/PluginDataGtk.cpp
	        plugins/gtk/PluginViewGtk.cpp
    	    plugins/gtk/gtk2xtbin.c
	    )
	ELSEIF (OWBAL_PLATFORM_GRAPHICS STREQUAL "SDL")
    	LIST (APPEND PLUGINS_SRC
	        plugins/SDL/PluginDatabaseSDL.cpp
    	    plugins/SDL/PluginPackageSDL.cpp
        	plugins/SDL/PluginDataSDL.cpp
	        plugins/SDL/PluginViewSDL.cpp
    	)
	ENDIF (OWBAL_PLATFORM_GRAPHICS STREQUAL "GTK")
ELSE (WEBKIT_USE_NPAPI)
    LIST (APPEND PLUGINS_SRC
        plugins/Empty/PluginDatabaseEmpty.cpp
        plugins/Empty/PluginDataEmpty.cpp
        plugins/Empty/PluginPackageEmpty.cpp
        plugins/Empty/PluginViewEmpty.cpp
    )
ENDIF (WEBKIT_USE_NPAPI) 

INCLUDE (plugins/Customer/CMakeLists.txt OPTIONAL)
