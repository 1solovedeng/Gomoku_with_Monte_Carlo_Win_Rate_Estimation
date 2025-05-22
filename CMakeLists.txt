# 最低 CMake 版本要求
cmake_minimum_required(VERSION 3.10)

# 项目信息
project(GomokuMonteCarlo VERSION 0.1.0 LANGUAGES C CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项
if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 可执行文件输出目录（Debug/Release 都输出到源码根目录）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

# 可执行目标名称
set(TARGET_NAME ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# 查找 SDL2 及其扩展库
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# 如果 find_package 失败，或者你想手动指定路径，取消下面注释并修改成你本地的安装目录
# if (WIN32)
#   include_directories("D:/SDL2/SDL2/include")
#   link_directories("D:/SDL2/SDL2/lib/x64")
# elseif (UNIX)
#   include_directories("/usr/include/SDL2")
#   link_directories("/usr/lib/x86_64-linux-gnu")
# elseif (APPLE)
#   include_directories("/usr/local/include/SDL2")
#   link_directories("/usr/local/lib")
# endif()

# 添加源文件
set(SOURCES
    SDLProject/main.cpp
    SDLProject/Board.cpp
    SDLProject/Game.cpp
)

# 生成可执行文件
add_executable(${TARGET_NAME} ${SOURCES})

# 包含 SDL2 头文件目录
target_include_directories(${TARGET_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})

# 链接 SDL2 库
target_link_libraries(${TARGET_NAME}
    PRIVATE
      ${SDL2_LIBRARIES}
    INTERFACE
      SDL2::SDL2_image
      SDL2::SDL2_mixer
      SDL2::SDL2_ttf
)
