cmake_minimum_required(VERSION 3.15)
project(MaskGenerator)

set(CMAKE_CXX_STANDARD 14)

IF (WIN32)
    set(OpenCV_DIR "D:/Libraries/OpenCV/4.1.2/build_vc16/install")
    set(Qt5_DIR "D:/Libraries/Qt/5.13.2/msvc2017_64/lib/cmake/Qt5")
ELSEIF (UNIX)
    set(OpenCV_DIR "/usr/lib/cmake/opencv4")
    set(Qt5_DIR "/usr/lib/cmake/Qt5")
    # set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
    # set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/usr/lib/cmake")
ENDIF()

# Find OpenCV
find_package(OpenCV REQUIRED)
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
# Add OpenCV headers location to your include paths
include_directories(${OpenCV_INCLUDE_DIRS})

#查找需要的Qt库
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS UiPlugin REQUIRED)
#find_package(Qt5 COMPONENTS Multimedia REQUIRED)
#find_package(Qt5 COMPONENTS MultimediaWidgets REQUIRED)

#Qt版本号
message(STATUS "Qt5Widgets_VERSION:${Qt5Widgets_VERSION}")
#Qt的includes
message(STATUS "Qt5Widgets_INCLUDE_DIRS:${Qt5Widgets_INCLUDE_DIRS}")
#取include的根目录,是列表变量的第一个元素
list(GET Qt5Widgets_INCLUDE_DIRS 0 Qt5_INCLUDE_DIR)
message(STATUS "Qt5Widgets_INCLUDE_DIR:${Qt5_INCLUDE_DIR}")

#include_directories(${Qt5_INCLUDE_DIR}QtMultimediaWidgets/)
#include_directories(${Qt5_INCLUDE_DIR}QtMultimedia/)
include_directories(${Qt5_INCLUDE_DIR}QtCore/${Qt5Widgets_VERSION}/QtCore/private/)
include_directories(${Qt5_INCLUDE_DIR}QtCore/${Qt5Widgets_VERSION}/QtCore/)
include_directories(${Qt5_INCLUDE_DIR}QtCore/${Qt5Widgets_VERSION}/)

#设置工程包含当前目录
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_definitions(-D UNICODE)
add_definitions(-D QT_CORE_LIB)
add_definitions(-D QT_GUI_LIB)
add_definitions(-D QT_WIDGETS_LIB)
#add_definitions(-D QT_MULTIMEDIA_LIB)
#add_definitions(-D QT_MULTIMEDIAWIDGETS_LIB)
add_definitions(-D QT_IMAGE_LIB)
add_definitions(-D QT_CORE_PRIVATE)
add_definitions(-D OPENCV)

#查找当前文件夹中的所有源代码文件
FILE(GLOB SRC_FILES "./*.cpp")
#查找当前文件夹中所有的头文件
FILE(GLOB HEAD_FILES "./*.h")
#查找当前文件夹中所有的ui文件
FILE(GLOB UI_FILES "./*.ui")
#查找当前文件夹中所有的ui文件
FILE(GLOB RES_FILES "./*.qrc")
#编译ui文件
qt5_wrap_ui(UIC_FILES ${UI_FILES})
#moc
qt5_wrap_cpp(MOC_FILES ${HEAD_FILES})
#编译qrc资源文件
qt5_add_resources(QRC_FILES ${RES_FILES})

#文件整理
source_group("Generated Files" FILES ${UIC_FILES} ${MOC_FILES} ${QRC_FILES})
source_group("Form Files" FILES ${UI_FILES})
source_group("Resource Files" FILES ${RES_FILES})
source_group("Source Files" FILES ${SRC_FILES})
source_group("Header Files" FILES ${HEAD_FILES})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ../../install/lib/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../install/lib/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ../../install/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../install/bin/Release)

#创建可执行文件工程
add_executable(MaskGenerator ${SRC_FILES} ${HEAD_FILES} ${MOC_FILES} ${RES_FILES} ${UIC_FILES} ${QRC_FILES})

#链接Qt5依赖项
target_link_libraries(MaskGenerator Qt5::Widgets Qt5::Core Qt5::Gui)
#target_link_libraries(MaskGenerator Qt5::Multimedia Qt5::MultimediaWidgets)
#链接OpenCV
target_link_libraries(MaskGenerator ${OpenCV_LIBS})