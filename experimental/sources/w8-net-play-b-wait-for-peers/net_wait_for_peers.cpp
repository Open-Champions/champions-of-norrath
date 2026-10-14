// Observed netWaitForPeersToCatchUp at 0x0012b198, 180 bytes. It first stores
// 3 to engineDisplayDisable. Then, up to 121 times, it scans the first
// nmPartners partner records (stride 17632): a record is behind when the
// signed bytes at 17620 and 17621 are both zero and the signed word at offset
// 17600 is below trueNetFrameNm. If no record is behind the function returns.
// If one is behind and fewer than 121 steps have run, it calls
// netGameStep(true, true) and engineSyncVBlank() and scans again, reloading
// nmPartners each time. The 122nd scan returns even if a record is still
// behind.
// Inferred: the word at 17600 is the last frame number heard from that peer.
// Unknown: what the display-disable value 3 does, and why the step is called
// with both flags set.
extern "C" {
extern int engineDisplayDisable;
extern int nmPartners;
extern int trueNetFrameNm;
extern unsigned char partners[];
void netGameStep__FbT0(bool first, bool second);
void engineSyncVBlank__Fv();
}

extern "C" void netWaitForPeersToCatchUp__Fv() {
    engineDisplayDisable = 3;
    int steps = 0;
    for (;;) {
        bool behind = false;
        int count = nmPartners;
        for (int i = 0; i < count; ++i) {
            const unsigned char* record = partners + i * 17632;
            if (static_cast<signed char>(record[17620]) != 0) continue;
            if (static_cast<signed char>(record[17621]) != 0) continue;
            if (*reinterpret_cast<const int*>(record + 17600) < trueNetFrameNm) behind = true;
        }
        if (!behind) return;
        if (!(steps < 121)) return;
        ++steps;
        netGameStep__FbT0(true, true);
        engineSyncVBlank__Fv();
    }
}
