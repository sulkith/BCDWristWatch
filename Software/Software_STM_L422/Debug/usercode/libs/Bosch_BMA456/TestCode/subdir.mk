################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456/TestCode/TestCode.cpp \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456/TestCode/main.cpp \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456/TestCode/main_SPI.cpp 

OBJS += \
./usercode/libs/Bosch_BMA456/TestCode/TestCode.o \
./usercode/libs/Bosch_BMA456/TestCode/main.o \
./usercode/libs/Bosch_BMA456/TestCode/main_SPI.o 

CPP_DEPS += \
./usercode/libs/Bosch_BMA456/TestCode/TestCode.d \
./usercode/libs/Bosch_BMA456/TestCode/main.d \
./usercode/libs/Bosch_BMA456/TestCode/main_SPI.d 


# Each subdirectory must supply rules for building sources it contributes
usercode/libs/Bosch_BMA456/TestCode/TestCode.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456/TestCode/TestCode.cpp usercode/libs/Bosch_BMA456/TestCode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456" -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
usercode/libs/Bosch_BMA456/TestCode/main.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456/TestCode/main.cpp usercode/libs/Bosch_BMA456/TestCode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456" -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
usercode/libs/Bosch_BMA456/TestCode/main_SPI.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456/TestCode/main_SPI.cpp usercode/libs/Bosch_BMA456/TestCode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456" -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usercode-2f-libs-2f-Bosch_BMA456-2f-TestCode

clean-usercode-2f-libs-2f-Bosch_BMA456-2f-TestCode:
	-$(RM) ./usercode/libs/Bosch_BMA456/TestCode/TestCode.d ./usercode/libs/Bosch_BMA456/TestCode/TestCode.o ./usercode/libs/Bosch_BMA456/TestCode/main.d ./usercode/libs/Bosch_BMA456/TestCode/main.o ./usercode/libs/Bosch_BMA456/TestCode/main_SPI.d ./usercode/libs/Bosch_BMA456/TestCode/main_SPI.o

.PHONY: clean-usercode-2f-libs-2f-Bosch_BMA456-2f-TestCode
