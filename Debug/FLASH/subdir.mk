################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FLASH/w25flash.c 

OBJS += \
./FLASH/w25flash.o 

C_DEPS += \
./FLASH/w25flash.d 


# Each subdirectory must supply rules for building sources it contributes
FLASH/%.o FLASH/%.su FLASH/%.cyclo: ../FLASH/%.c FLASH/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/YCZN_MT/STM32CubeIDE/workspace_1.18.0/Demo12_1_FlashFAT/KEY_LED" -I"C:/Users/YCZN_MT/STM32CubeIDE/workspace_1.18.0/Demo12_1_FlashFAT/FLASH" -I"C:/Users/YCZN_MT/STM32CubeIDE/workspace_1.18.0/Demo12_1_FlashFAT/FILE_TEST" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FLASH

clean-FLASH:
	-$(RM) ./FLASH/w25flash.cyclo ./FLASH/w25flash.d ./FLASH/w25flash.o ./FLASH/w25flash.su

.PHONY: clean-FLASH

