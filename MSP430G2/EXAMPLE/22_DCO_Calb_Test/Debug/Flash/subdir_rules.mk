################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Flash/Flash.obj: ../Flash/Flash.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/software/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/software/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/software/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/include" --include_path="C:/Users/SAPL108/Desktop/G2 pocket lab code for CCS6.2/22_DCO_Calb_Test/Flash" -g --define=__MSP430G2553__ --diag_warning=225 --display_error_number --abi=eabi --printf_support=minimal --preproc_with_compile --preproc_dependency="Flash/Flash.d" --obj_directory="Flash" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


