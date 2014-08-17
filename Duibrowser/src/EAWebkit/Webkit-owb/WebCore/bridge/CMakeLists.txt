LIST (APPEND BRIDGE_SRC
    bridge/runtime_array.cpp
    bridge/runtime_method.cpp
    bridge/runtime_object.cpp
    bridge/runtime_root.cpp
    bridge/runtime.cpp
    bridge/NP_jsobject.cpp
    bridge/c/c_class.cpp
    bridge/c/c_instance.cpp
    bridge/c/c_runtime.cpp
    bridge/c/c_utility.cpp
    bridge/npruntime.cpp
    bridge/bal/bal_class.cpp
    bridge/bal/bal_instance.cpp
    bridge/bal/bal_runtime.cpp
)

    ADD_EXECUTABLE (testbalbindings
        bridge/testbalbindings.cpp)
    TARGET_LINK_LIBRARIES (testbalbindings
        jscore
        generated-sources
        webkit
        webcore-owb
        bal-events
        bal-facilities
        bal-fonts
        bal-graphics
        bal-imagedecoder
        bal-internationalization
        bal-media
        bal-memory
        bal-network
        bal-types
        bal-widgets
        ${DEEPSEE_LINK}
    )

IF (WEBKIT_USE_DATABASE)
    TARGET_LINK_LIBRARIES (testbalbindings
        bal-database
    )
ENDIF (WEBKIT_USE_DATABASE)

TARGET_LINK_LIBRARIES (testbalbindings
        -lxslt -lz -lm -lxml2
        ${GTHREAD_LIBRARIES}
	${XT_LIBRARIES}
    )
IF (${OWBAL_PLATFORM_GRAPHICS} STREQUAL "GTK")
    TARGET_LINK_LIBRARIES (testbalbindings ${GTK2_LIBRARIES})
ELSEIF (${OWBAL_PLATFORM_GRAPHICS} STREQUAL "SDL")
    TARGET_LINK_LIBRARIES (testbalbindings ${SDL_LIBRARIES})
ENDIF (${OWBAL_PLATFORM_GRAPHICS} STREQUAL "GTK")
