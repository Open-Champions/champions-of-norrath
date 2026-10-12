#include <cstring>

extern "C" {
struct FontPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* first_tag;
};
FontPacketState p;
unsigned char* glistp;
void* curFont;
int drawBucket;
int nmChars;
int scissored;
unsigned char* vifDirectQWordCount;
unsigned char* reglistLoopCount;
int nmDirectQWords;
int engineScissory1;
int engineScissory2;
signed char coreLoadingScreenRunning;
void fontEnd__Fv();
}

alignas(16) static unsigned char buf[1024];
static int scissor_calls;
static int sc[4];
static int add_calls;
static void* add_packet;
static int add_bucket;
static void* add_texture;
static void* add_record;
static int add_flag;
static unsigned char* glistp_seen_by_add;

extern "C" {
void fontScissor__Fiiii(int x0, int y0, int x1, int y1) {
    ++scissor_calls;
    sc[0] = x0; sc[1] = y0; sc[2] = x1; sc[3] = y1;
    // Emulates what a real scissor does to the shared state: 48 more bytes of
    // packet, the glyph count flushed into the totals, the run reopened.
    nmDirectQWords += nmChars * 2 + 3;
    nmChars = 0;
    p.cursor += 48;
}
void drawAddToList__FPviP8_textureP11_drawRecordi(void* packet, int bucket, void* texture, void* record, int flag) {
    ++add_calls;
    add_packet = packet;
    add_bucket = bucket;
    add_texture = texture;
    add_record = record;
    add_flag = flag;
    glistp_seen_by_add = glistp;
}
}

static unsigned long long q(const unsigned char* at) {
    unsigned long long v;
    std::memcpy(&v, at, 8);
    return v;
}
static unsigned short h(const unsigned char* at) {
    unsigned short v;
    std::memcpy(&v, at, 2);
    return v;
}

static void setup(int chars, int scissor) {
    std::memset(buf, 0xee, sizeof buf);
    glistp = buf;
    p.start = buf;
    p.cursor = buf + 176;
    p.first_tag = buf + 16;
    vifDirectQWordCount = buf + 12;
    reglistLoopCount = buf + 128;
    unsigned long long glyph_tag = 0x2400000000008000ULL | 4;
    std::memcpy(buf + 128, &glyph_tag, 8);
    std::memset(buf + 12, 0, 4);
    nmChars = chars;
    nmDirectQWords = 9;
    scissored = scissor;
    scissor_calls = 0;
    add_calls = 0;
    drawBucket = 5;
    static unsigned char font[32];
    static int texture_token;
    void* t = &texture_token;
    std::memcpy(font + 16, &t, sizeof t);
    curFont = font;
    engineScissory1 = 11;
    engineScissory2 = 400;
    coreLoadingScreenRunning = 0;
}

int main() {
    // No scissor, 3 pending glyphs.
    setup(3, 0);
    fontEnd__Fv();
    if (scissor_calls != 0) return 1;
    if (h(buf + 12) != 9 + 6) return 2;
    if (q(buf + 128) != (0x2400000000008000ULL | (4 + 6))) return 4;
    // qwords = ((176 / 8) - 2) / 2 = 10, ID rewritten to 6
    if (q(buf) != 0x6000000AULL) return 5;
    if (add_calls != 1 || add_packet != buf || add_bucket != 5 || add_record != nullptr || add_flag != 1) return 6;
    void* want_texture;
    std::memcpy(&want_texture, static_cast<unsigned char*>(curFont) + 16, sizeof want_texture);
    if (add_texture != want_texture) return 7;
    if (glistp != buf + 176 || glistp_seen_by_add != buf + 176) return 8;   // updated before the call

    // With a scissor: the scissor runs first, and everything after uses its state.
    setup(2, 1);
    fontEnd__Fv();
    if (scissor_calls != 1 || sc[0] != 0 || sc[1] != 11 || sc[2] != 640 || sc[3] != 400) return 9;
    if (h(buf + 12) != 9 + 4 + 3) return 10;          // totals after the scissor, no pending glyphs
    if (q(buf) != (0x60000000ULL | (((176 + 48) / 8 - 2) / 2))) return 11;
    if (glistp != buf + 224 || add_packet != buf) return 12;
    if (q(buf + 128) != (0x2400000000008000ULL | 4)) return 13;    // the scissor mock did not touch it

    // The loading screen drops the packet: the tag is still closed, glistp is kept.
    setup(1, 0);
    coreLoadingScreenRunning = 1;
    fontEnd__Fv();
    if (add_calls != 0 || glistp != buf) return 14;
    if (q(buf) != 0x6000000AULL) return 15;
    if (h(buf + 12) != 9 + 2) return 16;
    return 0;
}
