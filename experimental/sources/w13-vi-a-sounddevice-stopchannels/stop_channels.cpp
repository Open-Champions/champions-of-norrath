// Observed: StopChannels__13VISoundDeviceUl at 0x011267e0 is 140 bytes. Arguments: device (a0,
// unused) and a 64-bit mask (a1). It computes two words from the mask: lo = the low 24 bits
// (zero-extended into an int) and hi = bits 24..55 (an arithmetic shift of mask << 8, so bit
// 55 supplies the sign of the 32-bit word; bits 56..63 are dropped). It then calls
// FindIopFunction__FUi(4). If that returns a non-null pointer p, it ORs lo into p[0] and hi
// into p[1]. If it returns null it stores hi into sIopFunctionData[1] (the overlay staging
// array at 0x011edf08) and lo into sIopFunctionData[0], and calls SetIopFunction__FUii(4, 2).
// Nothing is returned.
// Inferred: IOP function 4 stops sound channels; calls made before the pending request is
// sent are merged by OR-ing the masks, otherwise the request is staged and queued with two
// words. The 24 + 32 bit split matches Stop__13VISoundDevicei, which does the same split for
// function 9.
// Unknown: the name of IOP function 4, why bits 56..63 are dropped, and the exact lifetime of
// the pending entry returned by FindIopFunction__FUi.
extern "C" {
int* FindIopFunction__FUi(unsigned function);
void SetIopFunction__FUii(unsigned function, int count);
extern int norrath_overlay_sIopFunctionData_11edf08[];

void StopChannels__13VISoundDeviceUl(void* device, unsigned long long channels) {
    (void)device;
    int* pending = FindIopFunction__FUi(4);
    int lo = static_cast<int>(channels & 0xffffff);
    int hi = static_cast<int>(static_cast<long long>(channels << 8) >> 32);
    if (pending) {
        pending[0] |= lo;
        pending[1] |= hi;
    } else {
        norrath_overlay_sIopFunctionData_11edf08[1] = hi;
        norrath_overlay_sIopFunctionData_11edf08[0] = lo;
        SetIopFunction__FUii(4, 2);
    }
}
}
