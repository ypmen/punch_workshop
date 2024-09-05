#include "krnl_mm2s.h"

#include <fstream>

#define NELEMENT 64
#define NCHANNEL 12
#define NPOL 2

#define N 4
#define NPACKET_PER_BLOCK 64

int main()
{
	std::ifstream datafile("testvector_data.dat", std::ios::binary);
	ap_uint<DWIDTH> *data = new ap_uint<DWIDTH> [N*NCHANNEL*NPOL*NELEMENT*NPACKET_PER_BLOCK];
	datafile.read((char *)data, sizeof(complex_t)*N*NCHANNEL*NPOL*NELEMENT*NPACKET_PER_BLOCK);

	std::ifstream userfile("testvector_user.dat", std::ios::binary);
	ap_uint<128> *user = new ap_uint<128> [N*NCHANNEL*NPOL*NELEMENT*NPACKET_PER_BLOCK];
	userfile.read((char *)user, sizeof(complex_t)*N*NCHANNEL*NPOL*NELEMENT*NPACKET_PER_BLOCK);

	hls::stream<pkt> out;

	krnl_mm2s(data, user, out, N*NCHANNEL*NPOL*NELEMENT*NPACKET_PER_BLOCK, 0);

	delete [] data;
	delete [] user;

	return 0;
}
