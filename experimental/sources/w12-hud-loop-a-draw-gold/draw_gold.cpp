// Observed drawGold__Fv (local, not exported) at 0x001c45d0, 1148 bytes, 320-byte frame. Returns at once
// when gameNoDraw is nonzero or the byte hudNoDraw is nonzero. Otherwise three passes.
// Setup: fontStart(defaultFont, 6, 0x804080e4, -1). goldNumber is an array of 8 records of 36 bytes:
// floats position x,y,z at +0,+4,+8; floats velocity at +12,+16,+20; int stamp at +24; int amount
// at +28; signed byte player at +32. Constants: k1 = bits 0x3bd099e0, k2 = bits 0x3db3be9b.
// Pass 1, records 0 to 7: skipped unless netPlayerIsLocal(player) is nonzero and the amount is nonzero.
// Then playerGettinIt[player] = 60; sprintf(buffer, "%d", amount); position += velocity (stored);
// the goal is (playerGoldx[player], playerGoldy[player], 0) as floats; with d = goal - position,
// s = velocity + d * k1 and velocity = s - s * k2 for each axis (z of the goal is zero). The alpha is
// 128 - 3 * (frameNm - stamp), limited to 128 above and 0 below; fontColor(alpha << 24 | 0x4080e4, -1,
// 0); fontPrint((int)x, (int)y, buffer, 100000).
// Pass 2, players 0 to 3, only for local players: when playerGettinIt[p] is nonzero it drops by one and
// goldAlpha[p] rises by 4 while below 128; otherwise goldAlpha[p] drops by 4 when above zero (the
// pass moves to the next player when it is already not above zero). When goldAlpha[p] is above zero:
// fontColor(alpha << 24 | 0x4080e4, -1, 0); sprintf(buffer, "%d", the int at player record byte 232);
// commaString(buffer); players 1 and 3 print at x = playerGoldx - fontStringSize(defaultFont, buffer,
// 100000), players 0 and 2 at x = playerGoldx, all at y = playerGoldy, limit 100000. fontEnd().
// Pass 3, players 0 to 3 with goldAlpha[p] above zero: texture = lumpFindResource("hud.lmp",
// "gold03.tex"); textureDrawColor(texture, playerGoldIconx[p], playerGoldy[p] - 10, 6,
// goldAlpha[p] << 24 | 0x808080).
// Inferred: floating gold amounts that fly toward each player's gold counter, then the counter and an
// icon. Unknown: the names of the record fields, and what produces the records.
extern "C" {
extern int gameNoDraw;
extern unsigned char hudNoDraw;
extern void* defaultFont;
extern int frameNm;
extern unsigned char goldNumber[];
extern int goldAlpha[];
extern int playerGettinIt[];
extern int playerGoldx[];
extern int playerGoldy[];
extern int playerGoldIconx[];
extern unsigned char player[];
int netPlayerIsLocal__Fi(int player);
void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last);
int sprintf(char* buffer, const char* format, ...);
void fontColor__FUiii(unsigned color, int a, int b);
void fontPrint__FiiPci(int x, int y, char* text, int limit);
void commaString__FPc(char* text);
int fontStringSize__FP4FontPci(void* font, char* text, int count);
void fontEnd__Fv();
void* lumpFindResource__FPcT0(const char* group, const char* name);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color);
}

namespace {
float float_from_bits(unsigned bits) {
    union {
        unsigned word;
        float value;
    } u;
    u.word = bits;
    return u.value;
}

unsigned load_u32(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

void store_u32(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}

float load_f32(const unsigned char* address) { return float_from_bits(load_u32(address)); }

void store_f32(unsigned char* address, float value) {
    union {
        unsigned word;
        float number;
    } u;
    u.number = value;
    store_u32(address, u.word);
}

unsigned color_with_alpha(int alpha) { return (static_cast<unsigned>(alpha) << 24) | 0x4080e4u; }
}

extern "C" void drawGold__Fv() {
    if (gameNoDraw != 0) return;
    if (static_cast<signed char>(hudNoDraw) != 0) return;
    const float k1 = float_from_bits(0x3bd099e0u);
    const float k2 = float_from_bits(0x3db3be9bu);
    char buffer[64];
    fontStart__FP4FontiUii(defaultFont, 6, 0x804080e4u, -1);
    for (int index = 0; index < 8; ++index) {
        unsigned char* record = goldNumber + index * 36;
        const int owner = static_cast<signed char>(record[32]);
        if (netPlayerIsLocal__Fi(owner) == 0) continue;
        const int amount = static_cast<int>(load_u32(record + 28));
        if (amount == 0) continue;
        playerGettinIt[owner] = 60;
        sprintf(buffer, "%d", amount);
        const float x = load_f32(record + 0) + load_f32(record + 12);
        store_f32(record + 0, x);
        const float goalY = static_cast<float>(playerGoldy[owner]);
        const float goalX = static_cast<float>(playerGoldx[owner]);
        const float y = load_f32(record + 4) + load_f32(record + 16);
        store_f32(record + 4, y);
        const float z = load_f32(record + 8) + load_f32(record + 20);
        const float dy = goalY - y;
        const float dx = goalX - x;
        const float dz = -z;
        store_f32(record + 8, z);
        const float sx = dx * k1;
        const float sy = dy * k1;
        const float sz = dz * k1;
        const float ty = load_f32(record + 16) + sy;
        const float tz = load_f32(record + 20) + sz;
        const float tx = load_f32(record + 12) + sx;
        store_f32(record + 16, ty - ty * k2);
        store_f32(record + 20, tz - tz * k2);
        store_f32(record + 12, tx - tx * k2);
        int alpha = 128 - 3 * (frameNm - static_cast<int>(load_u32(record + 24)));
        if (!(alpha < 129)) alpha = 128;
        if (alpha < 0) alpha = 0;
        fontColor__FUiii(color_with_alpha(alpha), -1, 0);
        fontPrint__FiiPci(static_cast<int>(load_f32(record + 0)), static_cast<int>(load_f32(record + 4)), buffer, 100000);
    }
    for (int p = 0; p < 4; ++p) {
        if (netPlayerIsLocal__Fi(p) == 0) continue;
        if (playerGettinIt[p] != 0) {
            int alpha = goldAlpha[p];
            playerGettinIt[p] = playerGettinIt[p] - 1;
            if (alpha < 128) goldAlpha[p] = alpha + 4;
        } else {
            if (goldAlpha[p] <= 0) continue;
            goldAlpha[p] = goldAlpha[p] - 4;
        }
        if (goldAlpha[p] <= 0) continue;
        fontColor__FUiii(color_with_alpha(goldAlpha[p]), -1, 0);
        sprintf(buffer, "%d", static_cast<int>(load_u32(player + p * 23984 + 232)));
        commaString__FPc(buffer);
        if (p == 1 || p == 3) {
            int width = fontStringSize__FP4FontPci(defaultFont, buffer, 100000);
            fontPrint__FiiPci(playerGoldx[p] - width, playerGoldy[p], buffer, 100000);
        } else {
            fontPrint__FiiPci(playerGoldx[p], playerGoldy[p], buffer, 100000);
        }
    }
    fontEnd__Fv();
    for (int p = 0; p < 4; ++p) {
        if (goldAlpha[p] <= 0) continue;
        void* texture = lumpFindResource__FPcT0("hud.lmp", "gold03.tex");
        textureDrawColor__FP8_textureiiiUi(texture, playerGoldIconx[p], playerGoldy[p] - 10, 6,
                                           (static_cast<unsigned>(goldAlpha[p]) << 24) | 0x808080u);
    }
}
