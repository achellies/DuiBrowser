ADD_DEFINITIONS(-D_UNICODE)
ADD_DEFINITIONS(-DWINCE)

INCLUDE_DIRECTORIES(${JAVASCRIPTCORE_DIR})

SET(casqt_sources
    casqt.cpp
    qbig5codec.cpp
    qeucjpcodec.cpp
    qeuckrcodec.cpp
    qgb18030codec.cpp
    qisciicodec.cpp
    qjiscodec.cpp
    qjpunicode.cpp
    qlatincodec.cpp
    qsimplecodec.cpp
    qsjiscodec.cpp
    qstring.cpp
    qtextcodec.cpp
    qtsciicodec.cpp
    qutfcodec.cpp
    ubrk.cpp
)

ADD_LIBRARY(casqt STATIC ${casqt_sources})
