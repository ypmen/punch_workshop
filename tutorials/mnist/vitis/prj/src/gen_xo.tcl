set TEMP_DIR  [lindex $::argv 0]

package_xo -kernel_name krnl_ai -kernel_xml src/kernel.xml -ip_directory ../../model_1/hls4ml_prj/myproject_prj/solution1/impl/ip -ctrl_protocol ap_ctrl_none -xo_path ${TEMP_DIR}/krnl_ai.xo