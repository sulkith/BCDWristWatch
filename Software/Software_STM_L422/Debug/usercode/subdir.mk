################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/ClockM.cpp \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/GForceUI.cpp \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/STM32L4_AnalogDisplayManager.cpp \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/STM32L4_BCDDisplayManager.cpp \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/STM32L4_HAL.cpp \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/SleepM.cpp \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/settings.cpp \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/stm32_hal_spi_adap.cpp \
/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/usermain.cpp 

OBJS += \
./usercode/ClockM.o \
./usercode/GForceUI.o \
./usercode/STM32L4_AnalogDisplayManager.o \
./usercode/STM32L4_BCDDisplayManager.o \
./usercode/STM32L4_HAL.o \
./usercode/SleepM.o \
./usercode/settings.o \
./usercode/stm32_hal_spi_adap.o \
./usercode/usermain.o 

CPP_DEPS += \
./usercode/ClockM.d \
./usercode/GForceUI.d \
./usercode/STM32L4_AnalogDisplayManager.d \
./usercode/STM32L4_BCDDisplayManager.d \
./usercode/STM32L4_HAL.d \
./usercode/SleepM.d \
./usercode/settings.d \
./usercode/stm32_hal_spi_adap.d \
./usercode/usermain.d 


# Each subdirectory must supply rules for building sources it contributes
usercode/ClockM.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/ClockM.cpp usercode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456" -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
usercode/GForceUI.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/GForceUI.cpp usercode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456" -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
usercode/STM32L4_AnalogDisplayManager.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/STM32L4_AnalogDisplayManager.cpp usercode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456" -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
usercode/STM32L4_BCDDisplayManager.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/STM32L4_BCDDisplayManager.cpp usercode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456" -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
usercode/STM32L4_HAL.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/STM32L4_HAL.cpp usercode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456" -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
usercode/SleepM.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/SleepM.cpp usercode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456" -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
usercode/settings.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/settings.cpp usercode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456" -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
usercode/stm32_hal_spi_adap.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/stm32_hal_spi_adap.cpp usercode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456" -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
usercode/usermain.o: /home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/usermain.cpp usercode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L422xx -c -I../Core/Inc -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode/libs/Bosch_BMA456" -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usercode

clean-usercode:
	-$(RM) ./usercode/ClockM.d ./usercode/ClockM.o ./usercode/GForceUI.d ./usercode/GForceUI.o ./usercode/STM32L4_AnalogDisplayManager.d ./usercode/STM32L4_AnalogDisplayManager.o ./usercode/STM32L4_BCDDisplayManager.d ./usercode/STM32L4_BCDDisplayManager.o ./usercode/STM32L4_HAL.d ./usercode/STM32L4_HAL.o ./usercode/SleepM.d ./usercode/SleepM.o ./usercode/settings.d ./usercode/settings.o ./usercode/stm32_hal_spi_adap.d ./usercode/stm32_hal_spi_adap.o ./usercode/usermain.d ./usercode/usermain.o

.PHONY: clean-usercode

