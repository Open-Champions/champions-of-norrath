#include "load_player_info.cpp"

#include <cstdio>
#include <cstring>
#include <string>

char importInfoHeader[112];
int camera_object;
void* gameCam = &camera_object;
static std::string log_text;

extern "C" void MCA_Cmd__Fiiii(int command, const void* text, int third, int fourth) {
    char b[96];
    unsigned long v = reinterpret_cast<unsigned long>(text);
    if (text == importInfoHeader) std::snprintf(b, sizeof b, "[%d,hdr,%d,%d]", command, third, fourth);
    else std::snprintf(b, sizeof b, "[%d,%lx,%d,%d]", command, v, third, fourth);
    log_text += b;
}
extern "C" int GetSaveFootPrint__6Camera(void* c) { log_text += c == &camera_object ? "[fp]" : "[badcam]"; return 1000; }
extern "C" int jumpGateSaveFootPrint__Fv() { log_text += "[jg]"; return 24; }

int main() {
    // ids == null: header read, then seeks relative 14 and whole-record reads
    log_text.clear();
    static unsigned char players[2][23984];
    loadPlayerInfo__FiPiP12playerStruct(2, nullptr, players);
    char want[400];
    unsigned long p0 = reinterpret_cast<unsigned long>(players[0]);
    unsigned long p1 = reinterpret_cast<unsigned long>(players[1]);
    std::snprintf(want, sizeof want,
        "[2,0,0,0][3,hdr,112,0][fp][jg][2,%lx,0,0][2,e,1,0][3,%lx,23984,0][2,e,1,0][3,%lx,23984,0]",
        1000ul + 24 + 4216, p0, p1);
    if (log_text != want) { std::puts(log_text.c_str()); std::puts(want); return 1; }
    // ids given: no header, skip grows by 23896, reads 4 bytes into each id slot
    log_text.clear();
    int ids[3];
    loadPlayerInfo__FiPiP12playerStruct(3, ids, players);
    unsigned long i0 = reinterpret_cast<unsigned long>(&ids[0]);
    std::snprintf(want, sizeof want,
        "[fp][jg][2,%lx,0,0][2,62,1,0][3,%lx,4,0][2,%lx,1,0][3,%lx,4,0][2,%lx,1,0][3,%lx,4,0]",
        1000ul + 24 + 4216, i0, 98ul + 23896, i0 + 4, 98ul + 2 * 23896, i0 + 8);
    if (log_text != want) { std::puts(log_text.c_str()); std::puts(want); return 2; }
    // zero entries: only the footprint seek
    log_text.clear();
    loadPlayerInfo__FiPiP12playerStruct(0, ids, players);
    std::snprintf(want, sizeof want, "[fp][jg][2,%lx,0,0]", 1000ul + 24 + 4216);
    if (log_text != want) return 3;
    std::puts("load player info passed");
    return 0;
}
