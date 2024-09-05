#include "cmdlineparser.h"
#include <iostream>
#include <fstream>
#include <cstring>

// XRT includes
#include "experimental/xrt_bo.h"
#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_mailbox.h"

int main(int argc, char** argv)
{
	// Command Line Parser
	sda::utils::CmdLineParser parser;

	// Switches
	//**************//"<Full Arg>",  "<Short Arg>", "<Description>", "<Default>"
	parser.addSwitch("--xclbin_file", "-x", "input binary file string", "");
	parser.addSwitch("--device_id", "-d", "device index", "0");
	parser.parse(argc, argv);

	// Read settings
	std::string binaryFile = parser.value("xclbin_file");
	std::string device_index = parser.value("device_id");

	if (argc < 3) {
		parser.printHelp();
		return EXIT_FAILURE;
	}

	std::cout << "Open the device" << device_index << std::endl;
	auto device = xrt::device(device_index);
	auto device_name = device.get_info<xrt::info::device::name>();
	char* xcl_mode = getenv("XCL_EMULATION_MODE");
	if ((xcl_mode != nullptr) && !strcmp(xcl_mode, "hw_emu")) {
		if (device_name.find("2018") != std::string::npos) {
			std::cout << "[INFO]: The example is not supported for " << device_name
					  << " for hw_emu. Please try other flows." << '\n';
			return EXIT_SUCCESS;
		}
	}
	if (device_name.find("u50_gen3x16_xdma_2019") != std::string::npos) {
		std::cout << "[INFO]: The example runs on non-CDMA mode for " << device_name << '\n';
	}
	std::cout << "Load the xclbin " << binaryFile << std::endl;
	auto uuid = device.load_xclbin(binaryFile);

	size_t size_in = 28*28;
	size_t size_out = 1;

	size_t size_in_bytes = size_in * 1;

	size_t size_out_bytes = size_out * 160;
	
	auto krnl_mm2s = xrt::kernel(device, uuid, "krnl_mm2s", 1);
	auto krnl_s2mm = xrt::kernel(device, uuid, "krnl_s2mm", 1);

	std::cout << "Allocate Buffer in Global Memory\n";
	auto bo_in = xrt::bo(device, size_in_bytes, krnl_mm2s.group_id(0));
	auto bo_out = xrt::bo(device, size_out_bytes, krnl_s2mm.group_id(0));

	// Map the contents of the buffer object into host memory
	auto bo_in_map = bo_in.map<char*>();
	auto bo_out_map = bo_out.map<char*>();

	std::ifstream datafile;
	datafile.open("python/input.dat", std::ios::binary);
	datafile.read((char *)bo_in_map, size_in_bytes);

	// Synchronize buffer content with device side
	std::cout << "synchronize input buffer data to device global memory\n";
	bo_in.sync(XCL_BO_SYNC_BO_TO_DEVICE);
	sleep(3);

	auto run_mm2s = xrt::run(krnl_mm2s);
	run_mm2s.set_arg(0, bo_in);
	run_mm2s.set_arg(2, size_in);

	auto run_s2mm = xrt::run(krnl_s2mm);
	run_s2mm.set_arg(0, bo_out);
	run_s2mm.set_arg(2, size_out);

	run_s2mm.start();
	run_mm2s.start();

	run_mm2s.wait();
	run_s2mm.wait();

	sleep(3);

	// Copy Result from Device Global Memory to Host Local Memory
	std::cout << "Getting Results..." << std::endl;
	bo_out.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

	sleep(3);

	std::cout<<"verify data..."<<std::endl;

	for (int k=0; k<10; k++)
	{
		std::cout<<((((unsigned int)(((unsigned char *)bo_out_map)[k*2+1]))<<8) + (unsigned int)(((unsigned char *)bo_out_map)[k*2])) / 1024.<<" ";
	}
	std::cout<<std::endl;

	return 0;
}
