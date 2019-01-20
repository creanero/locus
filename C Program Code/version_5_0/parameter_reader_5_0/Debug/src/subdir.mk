################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/error_checking_5_0.cpp \
../src/parameter_reader_4_0.cpp 

OBJS += \
./src/error_checking_5_0.o \
./src/parameter_reader_4_0.o 

CPP_DEPS += \
./src/error_checking_5_0.d \
./src/parameter_reader_4_0.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


