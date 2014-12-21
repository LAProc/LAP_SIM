################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/FMA.cpp \
../src/IO.cpp \
../src/Inv_Sqrt.cpp \
../src/LAPU.cpp \
../src/PE.cpp \
../src/Reg_File.cpp \
../src/Simulator.cpp 

OBJS += \
./src/FMA.o \
./src/IO.o \
./src/Inv_Sqrt.o \
./src/LAPU.o \
./src/PE.o \
./src/Reg_File.o \
./src/Simulator.o 

CPP_DEPS += \
./src/FMA.d \
./src/IO.d \
./src/Inv_Sqrt.d \
./src/LAPU.d \
./src/PE.d \
./src/Reg_File.d \
./src/Simulator.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


