#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"
#include "nnet_types.h"

#define DWIDTH 8

typedef ap_axiu<DWIDTH, 0, 0, 0> pkt;

typedef nnet::array<ap_fixed<8,3>, 1*1> input_t;

extern "C" {
void krnl_mm2s(
		ap_fixed<8,3> *in,
		hls::stream<pkt> &k2n,
		unsigned int size
);
}
