cmake_minimum_required(VERSION 3.13)
project(CS695_Health_Monitoring C)

set(CMAKE_C_STANDARD 11)

# EXPORT COMPILE COMMANDS REQUIRED FOR UNDERSTAND TOOL
#set(CMAKE_EXPORT_COMPILE_COMMANDS On)

# GDB DEBUGGING FLAG
#set(CMAKE_BUILD_TYPE RelWithDebInfo)


# Cross Compiler Configurations
#set(CMAKE_AR /usr/bin/arm-linux-gnueabihf-ar)
#set(CMAKE_ASM_COMPILER /usr/bin/arm-linux-gnueabihf-)
#set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc)
#set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)
#set(CMAKE_LINKER /usr/bin/arm-linux-gnueabihf-ld)
#set(CMAKE_OBJCOPY /usr/bin/arm-linux-gnueabihf-objcopy)
#set(CMAKE_RANLIB /usr/bin/arm-linux-gnueabihf-ranlib)
#set(CMAKE_SIZE /usr/bin/arm-linux-gnueabihf-size)
#set(CMAKE_STRIP /usr/bin/arm-linux-gnueabihf-strip)

# IF EMULATED BINMARY IS REQUIRED
#add_compile_definitions(EMULATOR)

add_executable(CS695_Health_Monitoring main.c)
