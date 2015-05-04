#=============================================================================#
# Author: QueezyTheGreat                                                      #
# Date:   26.04.2011                                                          #
#                                                                             #
# Description: Arduino CMake example                                          #
#                                                                             #
#=============================================================================#

# this line is needed only if you use boards with selectable cpu (like mega, pro etc)
# and this line should go before call to "cmake/ArduinoToolchain.cmake"
# this is durty hack and should be fixed somewhen, because it should go to 
# particular cmake subdirectory
### set(ARDUINO_CPU 16MHzatmega328)

set(CMAKE_TOOLCHAIN_FILE cmake/ArduinoToolchain.cmake) # Arduino Toolchain


cmake_minimum_required(VERSION 2.8)
#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(ArduinoExample C CXX)

print_board_list()
print_programmer_list()

# add libraries to project
link_directories(${ARDUINO_SDK}/libraries)

# add the project directory into build
add_subdirectory(robot_test_motors)
add_subdirectory(robot_test_encoders)
add_subdirectory(robot_test_encoders_noint)
add_subdirectory(robot_test_encoders_noint_analog)
add_subdirectory(robot_test_encoders_read_values)
add_subdirectory(robot_test_encoders_read_values2)
add_subdirectory(robot_test_encoders_read_values3)
add_subdirectory(robot_test_encoders_read_values4)
add_subdirectory(robot_test_sensors)
add_subdirectory(robot)
