cmake_minimum_required(VERSION 3.16)

project(testqml2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTORCC ON)

set(OpenCV_DIR "C:/OpenCV/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/lib/cmake/opencv4")

find_package(OpenCV REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 Multimedia QuickDialogs2 SerialPort )

include_directories(${OpenCV_INCLUDE_DIRS})

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(apptestqml2
    main.cpp
)

qt_add_qml_module(apptestqml2
    URI testqml2
    VERSION 1.0
    QML_FILES
        Main.qml
        QML_FILES HomePage.qml
        QML_FILES ParameterSettingPage.qml
        RESOURCES resources.qrc
        QML_FILES GeneratingPage.qml
        SOURCES DataHandler.h DataHandler.cpp
        SOURCES ImageProcessing.h ImageProcessing.cpp
        SOURCES Gcode.h Gcode.cpp
        SOURCES Serialization.h Serialization.cpp
        QML_FILES SerialMonitor.qml
        QML_FILES AdjustHomePosition.qml
        SOURCES
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(apptestqml2 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apptestqml2
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apptestqml2
    PRIVATE Qt6::Quick
    PRIVATE Qt6::QuickControls2
    PRIVATE Qt6::Multimedia
    PRIVATE Qt6::QuickDialogs2
    PUBLIC  ${OpenCV_LIBS}
    PRIVATE Qt6::SerialPort
)

include(GNUInstallDirs)
install(TARGETS apptestqml2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
