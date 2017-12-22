TEMPLATE =	lib

CONFIG += c++14
CONFIG += staticlib

QT -= core widgets gui network

INCLUDEPATH += "$$LIB_UTILS_DIR/include"
win32 {
INCLUDEPATH += "$(BOOST_ROOT)"
}

TARGET = "$$LIBFACADE_STORAGE_TRANSACTIONS"

HEADERS +=	facadeStorageTransactions.hpp

SOURCES +=      facadeStorageTransactions.cpp

CONFIG(debug, debug|release) {
    DESTDIR = "$$ROOT_DIR/build/debug/$$TARGET"
} else {
    DESTDIR = "$$ROOT_DIR/build/release/$$TARGET"
}

OBJECTS_DIR = \""$$DESTDIR/.obj\""


# source code encoding
QMAKE_CXXFLAGS += -finput-charset="UTF-8"
unix {
QMAKE_CXXFLAGS += -Werror
QMAKE_CXXFLAGS += -Wall -fno-strict-aliasing
}
