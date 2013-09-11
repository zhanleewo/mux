
SET(SRC_LIST main.cpp)
INCLUDE_DIRECTORIES(.)
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib /usr/local/lib)
ADD_EXECUTABLE(muxclient ${SRC_LIST})
TARGET_LINK_LIBRARIES(muxclient libcommon.a event event_core event_extra)
