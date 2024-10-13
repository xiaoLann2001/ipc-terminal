# code/src/CMakeLists.txt

# Set the source files for the GPIO dynamic library
file(GLOB LED_SRC "led/gpio.c")  # GPIO source file

# Create the dynamic library
add_library(gpio SHARED ${LED_SRC})  # Create shared library from source files

# Set the output directory for shared libraries
set_target_properties(gpio PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../lib"
)

# Set include directories for the GPIO library
target_include_directories(gpio PUBLIC
    ../include/led
)

# ps:当c++使用c库函数时，记得c头文件的函数声明用extern "C"修饰，否则会出现链接问题

# Optional: Specify any additional compile options for the GPIO library
target_compile_options(gpio PRIVATE
    -g -Wall  # Debugging and warning options
)

