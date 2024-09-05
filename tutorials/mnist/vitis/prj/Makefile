help:
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
	$(ECHO) "      Command to generate the design for specified Target and Shell."
	$(ECHO) ""
	$(ECHO) "  make run TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
	$(ECHO) "      Command to run application in emulation."
	$(ECHO) ""
	$(ECHO) "  make build TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
	$(ECHO) "      Command to build xclbin application."
	$(ECHO) ""
	$(ECHO) "  make host"
	$(ECHO) "      Command to build host application."
	$(ECHO) ""
	$(ECHO) "  make clean "
	$(ECHO) "      Command to remove the generated non-hardware files."
	$(ECHO) ""
	$(ECHO) "  make cleanall"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""

# Cleaning stuff
RM = rm -f
RMDIR = rm -rf

ECHO:= @echo

TARGET := hw

#Setting PLATFORM 
ifneq ($(PLATFORM),)
PLATFORM := $(PLATFORM)
endif

#   device2xsa - create a filesystem friendly name from device name
#   $(1) - full name of device
device2xsa = $(strip $(patsubst %.xpfm, % , $(shell basename $(PLATFORM))))

XSA :=
ifneq ($(PLATFORM), )
XSA := $(call device2xsa, $(PLATFORM))
endif

TEMP_DIR := ./_x.$(TARGET).$(XSA)
BUILD_DIR := ./build_dir.$(TARGET).$(XSA)
EMCONFIG_DIR = $(TEMP_DIR)

LINK_OUTPUT := $(BUILD_DIR)/krnl_ai.link.xclbin
PACKAGE_OUT = ./package.$(TARGET)

EXECUTABLE = ./host

HOST_SRCS += ../common_cpp/cmdlineparser.cpp ../common_cpp/logger.cpp ./src/host.cpp

############################## Setting up Host Variables ##############################
CXXFLAGS += -fmessage-length=0 -g -O0
CPPFLAGS += -I../common_cpp -I/opt/xilinx/xrt/include/
LDFLAGS += -L$(XILINX_XRT)/lib -pthread -lOpenCL
LDFLAGS += -lrt -lstdc++
LDFLAGS += -luuid -lxrt_coreutil

############################## Setting up Kernel Variables ##############################
# Kernel compiler global settings
VPP_FLAGS += --save-temps -g -R2
VPP_LDFLAGS += --config ./krnl_ai.cfg --kernel_frequency 100

.PHONY: all clean emconfig
all: check-platform check-vitis $(EXECUTABLE) $(BUILD_DIR)/krnl_ai.xclbin emconfig

.PHONY: host
host: $(EXECUTABLE)

############################## Cleaning Rules ##############################
# Cleaning stuff
clean:	SHELL := /bin/bash
clean:
	-$(RMDIR) $(EXECUTABLE) $(XCLBIN)/{*sw_emu*,*hw_emu*} 
	-$(RMDIR) profile_* TempConfig system_estimate.xtxt *.rpt *.csv 
	-$(RMDIR) src/*.ll *v++* .Xil emconfig.json dltmp* xmltmp* *.log *.jou *.wcfg *.wdb

cleanall: SHELL := /bin/bash
cleanall: clean
	-$(RMDIR) build_dir*
	-$(RMDIR) package.*
	-$(RMDIR) _x* *xclbin.run_summary qemu-memory-_* emulation _vimage pl* start_simulation.sh *.xclbin

$(BUILD_DIR)/krnl_ai.xclbin: $(TEMP_DIR)/krnl_ai.xo $(TEMP_DIR)/krnl_mm2s.xo $(TEMP_DIR)/krnl_s2mm.xo
	mkdir -p $(BUILD_DIR)
	v++ -l $(VPP_FLAGS) $(VPP_LDFLAGS) -t $(TARGET) --user_ip_repo_paths ../../model_1/hls4ml_prj/myproject_prj/solution1/impl/ip --platform $(PLATFORM) --temp_dir $(TEMP_DIR) -o'$(LINK_OUTPUT)' $(+)
	v++ -p $(LINK_OUTPUT) $(VPP_FLAGS) -t $(TARGET) --user_ip_repo_paths ../../model_1/hls4ml_prj/myproject_prj/solution1/impl/ip --platform $(PLATFORM) --package.out_dir $(PACKAGE_OUT) -o $(BUILD_DIR)/krnl_ai.xclbin

$(TEMP_DIR)/krnl_ai.xo:
	mkdir -p $(TEMP_DIR)
	vivado -mode batch -source src/gen_xo.tcl -tclargs $(TEMP_DIR)

$(TEMP_DIR)/krnl_mm2s.xo: ../common_hls/krnl_mm2s.cpp
	mkdir -p $(TEMP_DIR)
	v++ -c $(VPP_FLAGS) -t $(TARGET) --platform $(PLATFORM) -k krnl_mm2s --temp_dir $(TEMP_DIR)  -I'$(<D)' -o'$@' '$<'

$(TEMP_DIR)/krnl_s2mm.xo: ../common_hls/krnl_s2mm.cpp
	mkdir -p $(TEMP_DIR)
	v++ -c $(VPP_FLAGS) -t $(TARGET) --platform $(PLATFORM) -k krnl_s2mm --temp_dir $(TEMP_DIR)  -I'$(<D)' -o'$@' '$<'

############################## Setting Rules for Host (Building Host Executable) ##############################
$(EXECUTABLE): $(HOST_SRCS) | check-xrt
	g++ -o $@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)

emconfig: $(EMCONFIG_DIR)/emconfig.json
$(EMCONFIG_DIR)/emconfig.json:
	emconfigutil --platform $(PLATFORM) --od $(EMCONFIG_DIR)

############################## Setting Essential Checks and Running Rules ##############################
check-platform:
ifndef PLATFORM
	$(error PLATFORM not set. Please set the PLATFORM properly and rerun. Run "make help" for more details.)
endif

#Checks for XILINX_VITIS
check-vitis:
ifndef XILINX_VITIS
	$(error XILINX_VITIS variable is not set, please set correctly using "source <Vitis_install_path>/Vitis/<Version>/settings64.sh" and rerun)
endif

#Checks for XILINX_XRT
check-xrt:
ifndef XILINX_XRT
	$(error XILINX_XRT variable is not set, please set correctly using "source /opt/xilinx/xrt/setup.sh" and rerun)
endif
