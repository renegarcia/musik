TARGET = musik
TEMPLATE = app
LIBS += -lxmmsclient++ \
    -lkdeui
INCLUDEPATH += /usr/include/xmms2 \
    /usr/include/KDE \
    /usr/include \
    /usr/include/c++/4.3.3 \
    src/core \
    src/playlist \
    src/ui
HEADERS += src/core/coreplaylist.h \
    src/core/xmmsqt4.h \
    src/playlist/tablemodel.h \
    src/playlist/mediatablemodel.h \
    src/core/qxmms2.h \
    src/core/types.h \
    src/ui/playlistwidget.h \
    src/playlist/sortfilterproxymodel.h \
    src/ui/playlistmanagerwidget.h \
    src/ui/playlistwindow.h \
    src/ui/dplaylistmanagerwidget.h \
    src/ui/seekslider.h
SOURCES += src/core/xmmsqt4.cpp \
    src/core/coreplaylist.cpp \
    src/playlist/tablemodel.cpp \
    src/playlist/mediatablemodel.cpp \
    src/core/qxmms2.cpp \
    src/ui/playlistwidget.cpp \
    src/ui/kmain.cpp \
    src/playlist/sortfilterproxymodel.cpp \
    src/ui/playlistmanagerwidget.cpp \
    src/ui/playlistwindow.cpp \
    src/ui/dplaylistmanagerwidget.cpp \
    src/ui/seekslider.cpp
OTHER_FILES += brainstorm.txt \
    TODO.txt
