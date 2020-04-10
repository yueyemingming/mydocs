# CMakeLists.txt范例

- **QMake的 .pro 文件 转成CMake的 CMakeLists.txt 文件**

## 环境

- Ubuntu 16.04
- Qt5.12.3

## 范例

- 具体源码请查看 [弗锐思名人堂软件(私有仓库需rui-space登录)](https://github.com/rui-space/myfrs.halloffame/)

```bash
#设置cmake版本号
cmake_minimum_required (VERSION 3.1.0)

#设置工程名称
project (halloffame)

#项目设置------------------------------------------------------
#对Qt项目来说，设置自动生成moc文件，一定要设置
set(CMAKE_AUTOMOC ON)
#设置工程包含当前目录，非必须
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#设置c++11
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x")

#依赖设置------------------------------------------------------
#设置Qt依赖，这里手动设置了*.cmake的文件路径，有点土
set(Qt5Core_DIR /opt/Qt5.12.3/5.12.3/gcc_64/lib/cmake/Qt5Core)
#根据设置好的*.cmake文件路径，去找包
find_package(Qt5Core)
set(Qt5Gui_DIR /opt/Qt5.12.3/5.12.3/gcc_64/lib/cmake/Qt5Gui)
find_package(Qt5Gui)
set(Qt5Widgets_DIR /opt/Qt5.12.3/5.12.3/gcc_64/lib/cmake/Qt5Widgets)
find_package(Qt5Widgets)
set(Qt5Sql_DIR /opt/Qt5.12.3/5.12.3/gcc_64/lib/cmake/Qt5Sql)
find_package(Qt5Sql)
set(Qt5Network_DIR /opt/Qt5.12.3/5.12.3/gcc_64/lib/cmake/Qt5Network)
find_package(Qt5Network)

#自动查找系统Protobuf包,此处未手动设置*.cmake路径
find_package(Protobuf REQUIRED)

#添加源码文件，也可以通过Set命令将所有文件设置为一个变量
FILE(GLOB SRC_FILES "src/*.cpp" "src/*.cc")
FILE(GLOB HEAD_FILES "include/*.h")
FILE(GLOB UI_FILES "widget.ui")

#添加资源文件，此处通过set设置为一个变量
#set(RCC_FILES rcc.qrc)

#通过Ui文件生成对应的头文件，一定要添加，此句一定要写在find_package(Qt依赖包)下面，否则会找不到这个命令。
qt5_wrap_ui(WRAP_FILES ${UI_FILES})

#创建工程文件
add_executable (${PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES} ${WRAP_FILES} )

#设置需要的头文件路径
include_directories (include /usr/local/include /usr/local/protobuf/include)
#设置需要的动态库路径
link_directories (/usr/local/protobuf/lib)
#设置需要的库
target_link_libraries (${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Sql Qt5::Network ${PROTOBUF_LIBRARIES} jsoncpp )


#安装设置------------------------------------------------------
#此处对应make install
install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION /opt/lynx/${PROJECT_NAME})
install (FILES "${PROJECT_SOURCE_DIR}/bin/config.json" DESTINATION /opt/lynx/${PROJECT_NAME})
install (DIRECTORY "${PROJECT_SOURCE_DIR}/bin/ui" DESTINATION /opt/lynx/${PROJECT_NAME})
install (FILES "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.service" DESTINATION /etc/systemd/system)

#打包设置------------------------------------------------------
#此处对应dpkg -i *.deb
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME}")
set(CPACK_PACKAGE_DESCRIPTION "Hall Of Fame")

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "lynx@lynxattractions.com")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS 1)
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/debian/postinst;${CMAKE_CURRENT_SOURCE_DIR}/debian/prerm;${CMAKE_CURRENT_SOURCE_DIR}/debian/conffiles")

include(CPack)
```
