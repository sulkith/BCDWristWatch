################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../usercode/ClockM.cpp \
../usercode/GForceUI.cpp \
../usercode/STM32L4_AnalogDisplayManager.cpp \
../usercode/STM32L4_BCDDisplayManager.cpp \
../usercode/STM32L4_HAL.cpp \
../usercode/SleepM.cpp \
../usercode/settings.cpp \
../usercode/stm32_hal_spi_adap.cpp \
../usercode/usermain.cpp 

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
usercode/%.o: ../usercode/%.cpp usercode/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32L432xx -c -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I"/home/sulkith/git/BCDWristWatch/Software/Software_STM/usercode" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-usercode

clean-usercode:
	-$(RM) ./usercode/ClockM.d ./usercode/ClockM.o ./usercode/GForceUI.d ./usercode/GForceUI.o ./usercode/STM32L4_AnalogDisplayManager.d ./usercode/STM32L4_AnalogDisplayManager.o ./usercode/STM32L4_BCDDisplayManager.d ./usercode/STM32L4_BCDDisplayManager.o ./usercode/STM32L4_HAL.d ./usercode/STM32L4_HAL.o ./usercode/SleepM.d ./usercode/SleepM.o ./usercode/settings.d ./usercode/settings.o ./usercode/stm32_hal_spi_adap.d ./usercode/stm32_hal_spi_adap.o ./usercode/usermain.d ./usercode/usermain.o

.PHONY: clean-usercode

