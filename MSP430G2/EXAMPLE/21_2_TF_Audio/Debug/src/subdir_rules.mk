################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/DAC8411.obj: ../src/DAC8411.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/software/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/software/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/SAPL108/Desktop/G2 pocket lab code for CCS6.2/21_2_TF_Audio/src" --include_path="C:/Users/SAPL108/Desktop/G2 pocket lab code for CCS6.2/21_2_TF_Audio/src/SD" --include_path="C:/software/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/include" -g --define=__MSP430G2553__ --diag_warning=225 --display_error_number --abi=eabi --printf_support=minimal --preproc_with_compile --preproc_dependency="src/DAC8411.d" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/softFIFO.obj: ../src/softFIFO.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/software/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="C:/software/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/Users/SAPL108/Desktop/G2 pocket lab code for CCS6.2/21_2_TF_Audio/src" --include_path="C:/Users/SAPL108/Desktop/G2 pocket lab code for CCS6.2/21_2_TF_Audio/src/SD" --include_path="C:/software/ti/ccsv6/tools/compiler/msp430_15.12.3.LTS/include" -g --define=__MSP430G2553__ --diag_warning=225 --display_error_number --abi=eabi --printf_support=minimal --preproc_with_compile --preproc_dependency="src/softFIFO.d" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


