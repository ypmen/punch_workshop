#include "krnl_s2mm.h"

void krnl_s2mm(
		ap_fixed<16,6> *out,
		hls::stream<pkt> &n2k,
		unsigned int size
)
{
#pragma HLS INTERFACE ap_ctrl_chain port=return
#pragma HLS INTERFACE m_axi port = out offset = slave bundle = gmem0 max_read_burst_length=64 max_write_burst_length=64
#pragma HLS INTERFACE axis port = n2k
#pragma HLS INTERFACE s_axilite port = size

	pkt v;

	for (int i=0; i < size; i++)
	{
#pragma HLS PIPELINE II=1 rewind
		n2k.read(v);

		for (int k=0; k<DWIDTH/16; k++)
		{
			out[i * (DWIDTH/16) + k].range(15,0) = v.data.range(k*16+15, k*16);
		}
	}
}
