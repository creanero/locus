################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/error_checking_5_3.cpp \
../src/fits_to_csv_1_0.cpp 

OBJS += \
./src/error_checking_5_3.o \
./src/fits_to_csv_1_0.o 

CPP_DEPS += \
./src/error_checking_5_3.d \
./src/fits_to_csv_1_0.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


