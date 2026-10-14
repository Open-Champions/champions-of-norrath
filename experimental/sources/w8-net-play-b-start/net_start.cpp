// Observed netStart at 0x00135110, 60 bytes. It copies trueNetFrameNm into
// sentStatePacketFrameNm and localNetFrameNm, clears syncRandState, calls
// worldPlotRouteSetEnable(0), then clears noAdvanceTime and stores
// localNetFrameNm - 10 into derivedFrameNm. localNetFrameNm is read again
// after the call.
// Inferred: none beyond the field names, which are the image's own symbols.
// Unknown: what the offset of 10 frames represents.
extern "C" {
extern int trueNetFrameNm;
extern int sentStatePacketFrameNm;
extern int localNetFrameNm;
extern unsigned syncRandState;
extern int noAdvanceTime;
extern int derivedFrameNm;
void worldPlotRouteSetEnable__Fi(int enable);
}

extern "C" void netStart__Fv() {
    int frame = trueNetFrameNm;
    sentStatePacketFrameNm = frame;
    localNetFrameNm = frame;
    syncRandState = 0;
    worldPlotRouteSetEnable__Fi(0);
    noAdvanceTime = 0;
    derivedFrameNm = localNetFrameNm - 10;
}
