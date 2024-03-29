# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = IShow
QT += core gui printsupport
greaterThan(QT_MAJOR_VERSION, 5): QT += widgets
win32:RC_FILE = IShow.rc

HEADERS += \
    ishow.h \
	QLayerWidget.h \
    qmylabel.h \
    qmymovie.h \
    resource.h

SOURCES += \
    ishow.cpp \
	QLayerWidget.cpp \
    main.cpp \
    qmylabel.cpp \
    qmymovie.cpp

FORMS += \
    ishow.ui \
	QLayerWidget.ui

DISTFILES += \
    ishow_en.ts \
    ishow_zh.ts
