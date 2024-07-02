# cmake的版本号
cmake_minimum_required(VERSION 3.27.8)
# project 的名字
project(TestOpenGL)
# C++ version
set(CMAKE_CXX_STANDARD 17)

# 检查环境变量
# if (NOT DEFINED ENV{GLFW_HOME})
#     message(FATAL_ERROR "found no env named GLFW_HOME")
# endif()
# if (NOT DEFINED ENV{GLAD_HOME})
#     message(FATAL_ERROR "found no env named GLAD_HOME")
# endif()

# 暂存环境变量
# set(GLFW_HOME $ENV{GLFW_HOME})
# set(GLAD_HOME $ENV{GLAD_HOME})
#set(GLFW_HOME "/Users/oceanisher/Downloads/glfw-3.3.9.bin.MACOS")
#set(GLAD_HOME "/Users/oceanisher/Downloads/glad")
#set(GLM_HOME "/Users/oceanisher/Downloads/glm")
set(GLFW_HOME "third/glfw-3.3.9.bin.MACOS")
set(GLAD_HOME "third/glad")
set(GLM_HOME "third/glm")
set(ASSIMP_HOME "third/assimp")

# 设置头文件目录
include_directories("${GLFW_HOME}/include")
include_directories("${GLAD_HOME}/include")
include_directories("${GLM_HOME}/glm")
#if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
#    include_directories("${ASSIMP_HOME}/assimp_arm64-osx/include")
#    include_directories("${ASSIMP_HOME}/assimp_arm64-osx/lib")
#else ()
#    include_directories("${ASSIMP_HOME}/assimp_arm64-osx/include")
#    include_directories("${ASSIMP_HOME}/assimp_arm64-osx/lib")
#endif ()

# 添加 GLFW3 预编译库
#add_library(glfw SHARED IMPORTED
#        src/LightNormal/LightNormal.h)
#SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION "${GLFW_HOME}/lib-arm64/libglfw.3.dylib")
add_library(glfw SHARED IMPORTED)
#message("当前CPU指令集：${CMAKE_HOST_SYSTEM_PROCESSOR}")
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64")
    SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/${GLFW_HOME}/lib-x86_64/libglfw.3.dylib")
else ()
    SET_TARGET_PROPERTIES(glfw PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/${GLFW_HOME}/lib-arm64/libglfw.3.dylib")
endif ()



# 创建可执行文件
add_executable(TestOpenGL 
        main.cpp 
        "lib/glad.c"
        src/DrawTriangle.cpp
        include/LearnOpenGLHeader.h
        include/Shader.h
        include/stb_image.h
        src/LearnOpenGL.cpp
#        src/shader/MoveTriangle.cpp
#        src/TextureTest/TextureTest.cpp
        src/MatrixTest/MatrixTest.h
        src/MatrixTest/MatrixTest2.h
        src/LightNormal/LightNormal.h
        src/LightNormal/LightMulti.h
        include/MeshInfo.h
        include/ModelInfo.h
        src/Mesh/ModelImport.h
)

# 链接 GLFW GLAD OpenGL
target_link_libraries(TestOpenGL glfw "-framework OpenGL")