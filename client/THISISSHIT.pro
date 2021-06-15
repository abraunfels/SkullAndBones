#-------------------------------------------------
#
# Project created by QtCreator 2017-12-25T07:13:20
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = THISISSHIT
TEMPLATE = app

QMAKE_CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=0

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += D:/Libs/boost_1_66_0/boost_1_66_0
LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_system-mgw49-mt-x32-1_66.a
LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_thread-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_atomic-mgw49-mt-x32-1_66.a
LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_chrono-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_container-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_context-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_coroutine-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_date_time-mgw49-mt-x32-1_66.a
LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_exception-mgw49-mt-x32-1_66.a
LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_filesystem-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_graph-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_iostreams-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_locale-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_log-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_log_setup-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_math_c99-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_math_c99f-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_math_c99l-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_math_tr1-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_math_tr1f-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_math_tr1l-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_prg_exec_monitor-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_program_options-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_random-mgw49-mt-x32-1_66.a
LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_regex-mgw49-mt-x32-1_66.a
LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_serialization-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_signals-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_stacktrace_noop-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_stacktrace_windbg-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_test_exec_monitor-mgw49-mt-x32-1_66.a
LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_thread-mgw49-mt-x32-1_66.a
LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_timer-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_type_erasure-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_unit_test_framework-mgw49-mt-x32-1_66.a
#LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_wave-mgw49-mt-x32-1_66.a
LIBS += D:/Libs/boost_1_66_0/boost_1_66_0/stage/lib/libboost_wserialization-mgw49-mt-x32-1_66.a

win32:LIBS += -lsetupapi
win32:LIBS += -lwsock32
win32:LIBS += -lws2_32

win32:DEFINES += _WIN32_WINNT=0x0501

win32:RC_ICONS += mask.ico



SOURCES += \
        settings.cpp \
        authorize.cpp \
        chatwindow.cpp \
        client.cpp \
        main.cpp \
        adduser.cpp \
        list_item.cpp \
        letters.cpp


HEADERS += \
        settings.h \
        tcp_connection.h \
        head.h \
        authorize.h \
        chat_message.h \
        chatwindow.h \
        client.h \
        adduser.h \
        list_item.h \
        letters.h

FORMS += \
        settings.ui \
        authorize.ui \
        chatwindow.ui \
        adduser.ui \
        list_item.ui \
    letters.ui \
    messageframe.ui

RESOURCES += \
        resources.qrc

DISTFILES += \
    images/fon.png \
    images/fon.png
