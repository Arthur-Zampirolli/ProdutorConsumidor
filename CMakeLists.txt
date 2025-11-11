# Define a versão mínima do CMake necessária para configurar o projeto
cmake_minimum_required(VERSION 3.11)

project(ProdutorConsumidor C) # 'C' indica que este é um projeto C


set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)

if (CMAKE_C_COMPILER_ID MATCHES "GNU")

    add_compile_options(
        -Wall          
        -Wextra        
        -Wpedantic     
        -Werror        
    )
endif()

add_executable(
    main      
    src/main.c
)