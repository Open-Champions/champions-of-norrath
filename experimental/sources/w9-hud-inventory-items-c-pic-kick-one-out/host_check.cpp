// Host check for picKickOneOut__4itemv: candidate choice, in-progress exclusion, slot clearing.
extern "C" int picKickOneOut__4itemv();
extern "C" {
int _4item$picInProgress, drawFrameNm;
unsigned char _4item$picList[64 * 192];
unsigned char _4item$picBuffer[256 * 1280];
}
static unsigned char* rec(int i) { return _4item$picList + 192 * i; }
static void stamp(int i, int v) { rec(i)[0] = 1; *reinterpret_cast<int*>(rec(i) + 124) = v; }
static void list(int i, short a, short b, short c, short d) { short* p = reinterpret_cast<short*>(rec(i) + 64); p[0] = a; p[1] = b; p[2] = c; p[3] = d; }
static void reset() {
    for (unsigned i = 0; i < sizeof _4item$picList; ++i) _4item$picList[i] = 0;
    for (int i = 0; i < 256; ++i) _4item$picBuffer[1280 * i] = 1;
    for (int i = 0; i < 64; ++i) list(i, -1, -1, -1, -1);
    _4item$picInProgress = -1; drawFrameNm = 100;
}
int main() {
    reset();
    stamp(3, 50); stamp(5, 40); stamp(7, 40); stamp(9, 92);     // 92 is not below 100 - 8
    list(5, 10, 11, -1, -1);
    _4item$picInProgress = 3;
    if (picKickOneOut__4itemv() != 1) return 1;
    if (rec(5)[0] != 0 || rec(7)[0] != 1 || rec(3)[0] != 1 || rec(9)[0] != 1) return 2;   // first of the tied stamps
    if (_4item$picBuffer[1280 * 10] != 0 || _4item$picBuffer[1280 * 11] != 0) return 3;
    if (_4item$picBuffer[1280 * 12] != 1 || _4item$picBuffer[0] != 1) return 4;
    reset();
    stamp(0, 99); stamp(2, 92); drawFrameNm = 100;               // nothing below 92
    if (picKickOneOut__4itemv() != 0 || rec(0)[0] != 1) return 5;
    reset();
    stamp(4, 10); _4item$picInProgress = 4; drawFrameNm = 1000;  // the only candidate is in progress
    if (picKickOneOut__4itemv() != 0 || rec(4)[0] != 1) return 6;
    _4item$picInProgress = 5;
    list(4, -1, 20, -1, -1);                                      // empty list: no slot is touched
    if (picKickOneOut__4itemv() != 1 || rec(4)[0] != 0 || _4item$picBuffer[1280 * 20] != 1) return 7;
    reset();
    stamp(1, -5); stamp(2, -9); drawFrameNm = 0;                  // signed stamps against -8
    list(2, 0, 255, 1, -1);
    if (picKickOneOut__4itemv() != 1 || rec(1)[0] != 1 || rec(2)[0] != 0) return 8;
    if (_4item$picBuffer[0] || _4item$picBuffer[1280 * 255] || _4item$picBuffer[1280]) return 9;
    return 0;
}
