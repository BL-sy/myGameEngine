cmake_minimum_required(VERSION 3.17)
project(Glad LANGUAGES C)

# Glad 源文件（默认生成的 glad.c + 头文件）
add_library(Glad STATIC
    src/glad.c
    include/glad/glad.h
    include/KHR/khrplatform.h
)

# 对外暴露头文件目录
target_include_directories(Glad INTERFACE include)