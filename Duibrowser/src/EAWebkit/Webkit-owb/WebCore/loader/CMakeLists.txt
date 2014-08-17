LIST (APPEND LOADER_SRC
    loader/Cache.cpp
    loader/CachedCSSStyleSheet.cpp
    loader/CachedFont.cpp
    loader/CachedImage.cpp
    loader/CachedResource.cpp
    loader/CachedResourceClientWalker.cpp
    loader/CachedScript.cpp
    loader/CachedXBLDocument.cpp
    loader/CachedXSLStyleSheet.cpp
    loader/DocLoader.cpp
    loader/DocumentLoader.cpp
    # Compile ImageDocument.cpp before FTPDirectoryDocument.cpp (N800)
    loader/ImageDocument.cpp
    loader/FTPDirectoryDocument.cpp
    loader/FTPDirectoryParser.cpp
    loader/FormState.cpp
    loader/FrameLoader.cpp
    loader/MainResourceLoader.cpp
    loader/NavigationAction.cpp
    loader/NetscapePlugInStreamLoader.cpp
    loader/PluginDocument.cpp
    loader/ProgressTracker.cpp
    loader/Request.cpp
    loader/ResourceLoader.cpp
    loader/SubresourceLoader.cpp
    loader/TextDocument.cpp
    loader/TextResourceDecoder.cpp
    loader/loader.cpp

    loader/archive/ArchiveResourceCollection.cpp
    loader/archive/ArchiveFactory.cpp
    loader/archive/ArchiveResource.cpp
)

IF (WEBKIT_OFFLINE_WEB_APPLICATIONS)
    LIST (APPEND LOADER_SRC
        loader/appcache/ApplicationCacheResource.cpp
        loader/appcache/ApplicationCacheStorage.cpp
        loader/appcache/ManifestParser.cpp
        loader/appcache/ApplicationCacheGroup.cpp
        loader/appcache/ApplicationCache.cpp
        loader/appcache/DOMApplicationCache.cpp
    )

ENDIF (WEBKIT_OFFLINE_WEB_APPLICATIONS)

IF (WEBKIT_USE_HTML5_VIDEO)
    LIST (APPEND LOADER_SRC
        loader/MediaDocument.cpp
    )
ENDIF (WEBKIT_USE_HTML5_VIDEO)

