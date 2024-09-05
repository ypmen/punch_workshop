#include "krnl_mm2s.h"

void krnl_mm2s(
		ap_fixed<8,3> *in,
		hls::stream<pkt> &k2n,
		unsigned int size
)
{
#pragma HLS INTERFACE ap_ctrl_chain port=return
#pragma HLS INTERFACE m_axi port = in offset = slave bundle = gmem0 max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE axis port = k2n
#pragma HLS INTERFACE s_axilite port = size
	
	pkt v;

	for (int i=0; i < size; i++)
	{
#pragma HLS PIPELINE II=1 rewind

		v.data = in[i].range(7, 0);

		k2n.write(v);
  	}
}
