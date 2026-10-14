// Observed commProcess__Fv at 0x002ccc70, 276 bytes. Does nothing when
// netNetworkMode (signed byte) is zero or frontEndRunning is nonzero. Otherwise:
//   commRVThresh   = (int)(commThresh * 65536) * 32767 / 131072
//   commRVInternal = (int)(commRecord * 4 * 65536) * 25 / 65536
//   commPV         = (int)(commPlayback * 4 * 65536)
// (float products in the order commRecord*4 then *65536, commPlayback*4 then
// *65536; integer divisions truncate toward zero). Then, with
// n = netGetLocalPlayerNmSafe(0), bit n of commSendmask is set when commEcho is
// nonzero and cleared otherwise. Finally commProcessReal() runs with
// enableMallocWrapper raised, and enableMallocWrapper is cleared afterwards.
extern "C" {
extern signed char netNetworkMode;
extern int frontEndRunning;
extern float commRecord;
extern float commThresh;
extern float commPlayback;
extern int commEcho;
extern int commPV;
extern int commRVThresh;
extern int commRVInternal;
extern int commSendmask;
extern unsigned char enableMallocWrapper;
int netGetLocalPlayerNmSafe__Fi(int index);
void commProcessReal__Fv();

void commProcess__Fv() {
    if (netNetworkMode == 0) return;
    if (frontEndRunning != 0) return;
    float record = commRecord * 4.0f;
    int thresh = static_cast<int>(commThresh * 65536.0f);
    float playback = commPlayback * 4.0f;
    int echo = commEcho;
    int recordFixed = static_cast<int>(record * 65536.0f);
    commPV = static_cast<int>(playback * 65536.0f);
    commRVThresh = thresh * 32767 / 131072;
    commRVInternal = recordFixed * 25 / 65536;
    if (echo != 0) {
        commSendmask |= 1 << netGetLocalPlayerNmSafe__Fi(0);
    } else {
        commSendmask &= ~(1 << netGetLocalPlayerNmSafe__Fi(0));
    }
    enableMallocWrapper = 1;
    commProcessReal__Fv();
    enableMallocWrapper = 0;
}
}
