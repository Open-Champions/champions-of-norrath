// Host contract for drawListClear__Fv. Not an EE plan input.
#include "draw_list_clear.cpp"

#include <cstring>

int writeBuffer = 0;
int nmDrawUsed = 99;
unsigned char* drawHead[16];
alignas(16) unsigned char drawRecords[2048 * 16 * 2];

int main() {
    for (int buffer = 0; buffer < 2; ++buffer) {
        std::memset(drawRecords, 0xAA, sizeof drawRecords);
        std::memset(drawHead, 0, sizeof drawHead);
        writeBuffer = buffer;
        nmDrawUsed = 99;
        drawListClear__Fv();
        if (nmDrawUsed != 8) return 1;
        for (int n = 0; n < 16; ++n) {
            bool mine = n >= buffer * 8 && n < buffer * 8 + 8;
            if (mine) {
                unsigned char* want = drawRecords + (buffer * 2048 + (n - buffer * 8)) * 16;
                if (drawHead[n] != want) return 2;
                for (int b = 0; b < 16; ++b)
                    if (want[b] != (b >= 12 ? 0 : 0xAA)) return 3;
            } else if (drawHead[n] != nullptr) {
                return 4;
            }
        }
        int zeroed = 0;
        for (unsigned i = 0; i < sizeof drawRecords; ++i)
            if (drawRecords[i] == 0) ++zeroed;
        if (zeroed != 8 * 4) return 5;
    }
    return 0;
}
