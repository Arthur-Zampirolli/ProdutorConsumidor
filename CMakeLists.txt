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
find_package(OpenMP REQUIRED)
add_executable(
    main      
    src/main.c
    src/lib/data.c
    src/lib/io.c
    src/lib/matrix.c
    src/lib/producer/producer.c
    src/lib/consumer/consumer.c
    src/lib/consumer_producer/consumer_producer.c
    src/lib/data_buffer.c
    
)
target_link_libraries(main PRIVATE OpenMP::OpenMP_C)