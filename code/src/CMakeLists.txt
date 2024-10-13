# code/src/CMakeLists.txt

# Set the source files for the GPIO dynamic library
file(GLOB LED_SRC "led/gpio.c")  # GPIO source file

# Create the dynamic library
add_library(gpio SHARED ${LED_SRC})  # Create shared library from source files

# Set include directories for the GPIO library
target_include_directories(gpio PUBLIC
    ../include/led
)

# Optional: Specify any additional compile options for the GPIO library
target_compile_options(gpio PRIVATE
    -g -Wall  # Debugging and warning options
)

