################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
led_display/%.obj: ../led_display/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/ti/ccs1250/ccs/ccs_base/msp430/include" --include_path="C:/Users/linds/OneDrive/Desktop/2023-2024 Semester 2/Elec 327/FinalProject/GuitarHero430/Experimental_Work" --include_path="C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power=all --define=__MSP430G2553__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="led_display/$(basename $(<F)).d_raw" --obj_directory="led_display" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


