################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456/i2c/twi.c 

C_DEPS += \
./usercode/libs/Bosch_BMA456/i2c/twi.d 

OBJS += \
./usercode/libs/Bosch_BMA456/i2c/twi.o 


# Each subdirectory must supply rules for building sources it contributes
usercode/libs/Bosch_BMA456/i2c/twi.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456/i2c/twi.c usercode/libs/Bosch_BMA456/i2c/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usercode-2f-libs-2f-Bosch_BMA456-2f-i2c

clean-usercode-2f-libs-2f-Bosch_BMA456-2f-i2c:
	-$(RM) ./usercode/libs/Bosch_BMA456/i2c/twi.d ./usercode/libs/Bosch_BMA456/i2c/twi.o

.PHONY: clean-usercode-2f-libs-2f-Bosch_BMA456-2f-i2c

