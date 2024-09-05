#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"
#include "nnet_types.h"

#define DWIDTH 160

typedef ap_axiu<DWIDTH, 0, 0, 0> pkt;

typedef nnet::array<ap_fixed<16,6>, 10*1> result_t;

extern "C" {
void krnl_s2mm(
		ap_fixed<16,6> *out,
		hls::stream<pkt> &n2k,
		unsigned int size
);
}