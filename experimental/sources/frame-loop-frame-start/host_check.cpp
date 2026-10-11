#include <cstdio>
#include <cstring>

#define NORRATH_HOST_CONTRACT 1
#include "frame_start.cpp"

extern "C" {
unsigned draw[16];
unsigned drawFrameNm;
int fullFrameMode;
volatile unsigned char vblankField;
unsigned gstart;
unsigned glistp;
}

namespace {
unsigned failures;
int flush_mode;
unsigned packet_color;
int packet_second;
int packet_word;
char sequence[128];
int mutate_on_draw;
unsigned mutate_word0;
unsigned mutate_word1;
unsigned mutate_word2;
unsigned mutate_frame;
int mutate_mode;

void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

void note(const char* name) {
    if (sequence[0] != 0) std::strcat(sequence, ",");
    std::strcat(sequence, name);
}

void reset() {
    for (unsigned i = 0; i < 16u; ++i) draw[i] = 0x40u + i;
    drawFrameNm = 1024u;
    fullFrameMode = 0;
    vblankField = 0;
    gstart = 0;
    glistp = 1u;
    flush_mode = -1;
    packet_color = 0;
    packet_second = -1;
    packet_word = -1;
    sequence[0] = 0;
    mutate_on_draw = 0;
}
}  // namespace

extern "C" void lightVU0StoppedProcessing__Fv() { note("light"); }
extern "C" void frameStartPacket__FUibi(unsigned color, int second, int word) {
    note("packet");
    packet_color = color;
    packet_second = second;
    packet_word = word;
}
extern "C" void FlushCache(int mode) {
    note("flush");
    flush_mode = mode;
}
extern "C" void drawStart__Fv() {
    note("draw");
    if (mutate_on_draw) {
        draw[0] = mutate_word0;
        draw[1] = mutate_word1;
        draw[2] = mutate_word2;
        drawFrameNm = mutate_frame;
        fullFrameMode = mutate_mode;
    }
}

int main() {
    reset();
    draw[0] = 5u;
    vblankField = 0xffu;
    engineFrameStart__FUib(0x80000000u, 2);
    expect(std::strcmp(sequence, "light,packet,flush,draw") == 0, "call order");
    expect(packet_color == 0x80000000u && packet_second == 2 && packet_word == 0x42, "packet sees the original word");
    expect(flush_mode == 0, "cache flush mode");
    expect(draw[3] == 1u && draw[0] == 1u, "word 12 is 1 and word 0 ends as 1");
    expect(draw[5] == 0x42u && draw[4] == 0x41u, "nonzero word 0 copies the reloaded pair");
    expect(gstart == 0x3038f9e0u && glistp == gstart, "even frame number selects the first pointer");
    expect(draw[2] == 0xffffffffu && draw[1] == 0u, "zero mode sign-extends the field byte");

    reset();
    draw[0] = 0u;
    draw[2] = 111u;
    vblankField = 0x7fu;
    engineFrameStart__FUib(1u, 0);
    expect(packet_word == 111, "zero word 0 still passes the original word");
    expect(draw[5] == 0u && draw[4] == 0u, "zero word 0 clears the copy");
    expect(draw[2] == 0x7fu, "a positive field byte stays positive");

    reset();
    mutate_on_draw = 1;
    mutate_word0 = 9u;
    mutate_word1 = 0x55u;
    mutate_word2 = 0x66u;
    mutate_frame = 7u;
    mutate_mode = 3;
    draw[0] = 0u;
    draw[2] = 111u;
    vblankField = 0xffu;
    engineFrameStart__FUib(4u, 0);
    expect(packet_word == 111, "packet does not see drawStart's word");
    expect(draw[5] == 0x66u && draw[4] == 0x55u, "the copy uses the words drawStart left");
    expect(gstart == 0x303d3fa0u && glistp == gstart, "odd frame number selects the second pointer");
    expect(draw[2] == 0u && draw[1] == 3u, "a nonzero mode stores 0 and that mode");
    expect(draw[0] == 1u && draw[3] == 1u, "word 0 is 1 after drawStart");

    reset();
    mutate_on_draw = 1;
    mutate_word0 = 0u;
    mutate_word1 = 0x55u;
    mutate_word2 = 0x66u;
    mutate_frame = 8u;
    mutate_mode = 0;
    draw[0] = 4u;
    vblankField = 0x80u;
    engineFrameStart__FUib(4u, 0);
    expect(draw[5] == 0u && draw[4] == 0u, "drawStart can clear word 0 before the copy");
    expect(gstart == 0x3038f9e0u, "the reloaded frame number selects the pointer");
    expect(draw[2] == 0xffffff80u, "mode left at zero sign-extends 0x80");

    if (failures != 0) {
        std::fprintf(stderr, "%u frame-start checks failed\n", failures);
        return 1;
    }
    std::printf("frame-start checks passed\n");
    return 0;
}
