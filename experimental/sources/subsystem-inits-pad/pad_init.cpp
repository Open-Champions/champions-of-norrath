// padInit__Fv at 0x00167538, 292 bytes, read from the pinned SLUS-20565 image.
// Fresh compilation is not an original-byte match. No pad port has been opened
// on the EE.
//
// Observed calls, in order: scePadInit(0), sceMtapInit, then for ports 0 and 1
// scePadPortOpen(port, 0, pad_dma_buf + port * 1280) and sceMtapPortOpen(port).
// Then for those ports and slots 1 through 4, scePadPortOpen(port, slot,
// pad_dma_buf + port * 1280 + slot * 256). Then four pairs of memset(pad + i * 88,
// 0, 88) and memset(stateData + i * 8, 0, 8). Then padInitPadIndexMap__Fv.
//
// pad_dma_buf is 2560 bytes at 0x006e1240. pad is 352 bytes at 0x0034beb8.
// stateData is 32 bytes at 0x006e1c40. Those objects stay imports.
extern "C" {
void scePadInit(int mode);
void sceMtapInit();
void scePadPortOpen(int port, int slot, void* buffer);
void sceMtapPortOpen(int port);
void* memset(void* destination, int value, unsigned count);
void padInitPadIndexMap__Fv();

extern unsigned char pad_dma_buf[];
extern unsigned char pad[];
extern unsigned char stateData[];

void padInit__Fv() {
    scePadInit(0);
    sceMtapInit();
    for (int port = 0; port < 2; ++port) {
        scePadPortOpen(port, 0, pad_dma_buf + port * 1280);
        sceMtapPortOpen(port);
    }
    for (int port = 0; port < 2; ++port) {
        for (int slot = 1; slot < 5; ++slot) {
            scePadPortOpen(port, slot, pad_dma_buf + port * 1280 + slot * 256);
        }
    }
    for (int index = 0; index < 4; ++index) {
        memset(pad + index * 88, 0, 88u);
        memset(stateData + index * 8, 0, 8u);
    }
    padInitPadIndexMap__Fv();
}
}
