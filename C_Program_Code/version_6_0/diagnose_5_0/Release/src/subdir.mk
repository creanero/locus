################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/diagnose_5_0.cpp \
../src/error_checking_4_1_1.cpp 

OBJS += \
./src/diagnose_5_0.o \
./src/error_checking_4_1_1.o 

CPP_DEPS += \
./src/diagnose_5_0.d \
./src/error_checking_4_1_1.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


