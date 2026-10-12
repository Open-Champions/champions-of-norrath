// Observed networking storage in the pinned SLUS_205.65 image. Sizes come from
// the ELF symbol size where it is nonzero, and otherwise from the distance to
// the next symbol; the second kind is marked. Layouts of the buffers are not
// established here and no field is named.
//   netNmPartners        0x00483830  1 byte,  initial 1
//   nmPartners           0x0048385c  4 bytes, initial 0 (read as a signed count)
//   broadcastConnection  0x00483860  4 bytes, initial -1
//   commHandle           0x00486654  4 bytes, initial 0
//   commStatusHandle     0x00486658  4 bytes, initial 0
//   netWaitMsgHandler    0x00486ad0  4 bytes (gap), zero in the file's BSS
//   partners             0x004e2280  70528 bytes (gap) = 4 records of 17632
//   g_net_buf            0x004f3680  2048 bytes (gap), zero in the file's BSS
// nmPartners is volatile because the only reader here, netGetLocalPlayerNm,
// declares it that way.
extern "C" {
unsigned char netNmPartners = 1;
volatile int nmPartners = 0;
int broadcastConnection = -1;
int commHandle = 0;
int commStatusHandle = 0;
int netWaitMsgHandler;
unsigned char partners[70528];
unsigned char g_net_buf[2048];
}
