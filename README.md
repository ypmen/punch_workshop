![PUNCH](tutorials/mnist/punch.png "PUNCH")
# PUNCH4NFDI TA5 Workshop (Dresden, 2024)

This tutorial guides you through the process of converting a QKeras machine learning model into Vitis HLS code. You'll learn how to apply quantization-aware training (QAT) and model pruning using QKeras to optimize your model. Additionally, the tutorial covers the steps to transform the optimized model into Vitis HLS code using the HLS4ML framework, with a practical example using the MNIST dataset. You can also find the official hls4ml tutorial https://github.com/fastmachinelearning/hls4ml-tutorial.

## Requirements
- tensorflow/qkeras (build the ML model)
- HLS4ML (convert the QKeras model to HLS) https://github.com/fastmachinelearning/hls4ml
- Vitis_HLS 2022.2 (complie the HLS code and export the IP)
- Vitis/Vivado 2022.2 (generate the xclbin file that can run on Alveo card)
- Xilinx Runtime (XRT is a low level communication layer (APIs and drivers) between the host and the card.) https://xilinx.github.io/XRT/2022.2/html/index.html
- Development Target Platform (The deployment target platform is the communication layer physically implemented and flashed into the card.) https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/alveo/u55c.html
- pynq >3.0.1 (a Jupyter-based framework with Python APIs for using AMD Xilinx Adaptive Computing platforms) https://pynq.readthedocs.io/en/latest/
