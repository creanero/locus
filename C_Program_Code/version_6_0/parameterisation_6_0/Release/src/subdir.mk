################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/error_checking_6_1.cpp \
../src/parameterisation_6_1.cpp 

OBJS += \
./src/error_checking_6_1.o \
./src/parameterisation_6_1.o 

CPP_DEPS += \
./src/error_checking_6_1.d \
./src/parameterisation_6_1.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


