################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../usercode/libs/Bosch_BMA456/Bosch_BMA.cpp \
../usercode/libs/Bosch_BMA456/Bosch_BMA456_config_file.cpp \
../usercode/libs/Bosch_BMA456/spi_adap.cpp 

OBJS += \
./usercode/libs/Bosch_BMA456/Bosch_BMA.o \
./usercode/libs/Bosch_BMA456/Bosch_BMA456_config_file.o \
./usercode/libs/Bosch_BMA456/spi_adap.o 

CPP_DEPS += \
./usercode/libs/Bosch_BMA456/Bosch_BMA.d \
./usercode/libs/Bosch_BMA456/Bosch_BMA456_config_file.d \
./usercode/libs/Bosch_BMA456/spi_adap.d 


# Each subdirectory must supply rules for building sources it contributes
usercode/libs/Bosch_BMA456/%.o: ../usercode/libs/Bosch_BMA456/%.cpp usercode/libs/Bosch_BMA456/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L432xx -c -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usercode-2f-libs-2f-Bosch_BMA456

clean-usercode-2f-libs-2f-Bosch_BMA456:
	-$(RM) ./usercode/libs/Bosch_BMA456/Bosch_BMA.d ./usercode/libs/Bosch_BMA456/Bosch_BMA.o ./usercode/libs/Bosch_BMA456/Bosch_BMA456_config_file.d ./usercode/libs/Bosch_BMA456/Bosch_BMA456_config_file.o ./usercode/libs/Bosch_BMA456/spi_adap.d ./usercode/libs/Bosch_BMA456/spi_adap.o

.PHONY: clean-usercode-2f-libs-2f-Bosch_BMA456

