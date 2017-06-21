cmake_minimum_required(VERSION 3.1)

project(SmdWindow)

find_package(OpenGL REQUIRED)

add_executable(SmdWindow
    README.md
    source/CCommonDialogs.cpp
    source/CCommonDialogs.h
    source/CControl.cpp
    source/CControl.h
    source/CMainFrame.cpp
    source/CMainFrame.h
    source/COpenGL.cpp
    source/COpenGL.h
    source/CWin.cpp
    source/CWin.h
    source/data_types.h
    source/GLData.cpp
    source/GLData.h
    source/GLWindow.cpp
    source/GLWindow.h
    source/WinMain.cpp
    resource/resource.rc
    resource/resource.h
	)

target_link_libraries(SmdWindow
    ${OPENGL_LIBRARIES}
    )

target_compile_features(SmdWindow
	PRIVATE cxx_auto_type
	PRIVATE cxx_nullptr
	PRIVATE cxx_range_for
	)
