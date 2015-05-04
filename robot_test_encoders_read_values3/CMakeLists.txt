# Set a variable for commands below
set(PROJECT_NAME robot_test_encoders_read_values3)

# Define your project and language
project(${PROJECT_NAME} C CXX)

set(${PROJECT_NAME}_BOARD uno)

# Define the source code
set(${PROJECT_NAME}_SRCS encoders.cpp)

# Define the port for uploading code to the Arduino
set(${PROJECT_NAME}_PORT COM7)

# Command to generate code arduino firmware (.hex file)
generate_arduino_firmware(${PROJECT_NAME})
