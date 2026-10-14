#include "unlink_all.cpp"
#include <string>

static unsigned char emitter[512];
static unsigned char nodes[3][64];
static int left;
static std::string tr;
extern "C" void* RemHead__7VILList(void* l) {
    tr += "H" + std::to_string((unsigned char*)l - emitter) + ";";
    return left > 0 ? nodes[--left] : 0;
}

int main() {
    for (int k = 0; k <= 3; k++) {
        tr.clear(); left = k;
        for (int i = 0; i < 3; i++) for (int j = 0; j < 64; j++) nodes[i][j] = 0x77;
        UnlinkAllMotifBlenders__17VIParticleEmitter(emitter);
        std::string want;
        for (int i = 0; i <= k; i++) want += "H176;";
        if (tr != want) return 1;
        for (int i = 0; i < 3; i++) {
            bool cleared = i >= 3 - k ? false : false;
            (void)cleared;
            bool touched = i < k;
            for (int j = 0; j < 64; j++) {
                unsigned char exp = (touched && j >= 16 && j < 20) ? 0 : 0x77;
                if (nodes[i][j] != exp) return 2;
            }
        }
    }
    return 0;
}
