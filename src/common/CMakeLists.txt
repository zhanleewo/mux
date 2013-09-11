SET(SRC_LIST util.cpp)
SET(SRC_H util.h protocols.h)
ADD_LIBRARY(common STATIC ${SRC_H} ${SRC_LIST})
