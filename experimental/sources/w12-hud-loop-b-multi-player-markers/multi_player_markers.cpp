// Observed maybeDrawMultiPlayerMarkers__6Player at 0x001aa0d0, 720 bytes: one argument (the player
// object). With idx the word at object + 960 and record = player + idx * 23984:
//  - Nothing is drawn when the word at object + 1328 is nonzero, when nmPlayers is 1, when
//    gameMultiPlayerMarkersOn is zero, when netGetCameraCustomControl__Fi(idx) is nonzero and bit
//    0x400000 of the word at object + 12 is clear, when DLG_Active__Fv() is nonzero, or when the
//    float at record + 172 is not above zero.
//  - radius = isin(562 * (500 * idx + frameNm % 65536)) + 20.0 (one isin call); alpha = (int)(the same
//    isin value * 14.0 + 57.0). The marker colour is (0, 0, 0) when record float +172 divided by
//    float +176 is below 0.3333 and bit 2 of frameNm is set; otherwise it is per player (the three
//    integers below, in the order they are passed): idx 0 (182, 47, 58), idx 1 (24, 217, 30),
//    idx 2 (216, 219, 33), any other (71, 61, 209).
//  - Forty-five sprites, for s = 0, 8, .. 352: angle = (short)(int)(s * 182.04443359375), drawn with
//    P_FaceSprite__FG6Point3ffiiiiP8_texturesi at position + (isin(angle) * radius, icos(angle) * radius,
//    0), size 6.0, depth 1822.0, the colour, alpha, texture haloTexture5, flags 0, mode 0.
//    isin is called before icos for each sprite.
// Inferred: this ring of dots marks a remote player's position for the others; the colour belongs to
// the player slot and flickers black when the float ratio (likely health over maximum) is low.
// Unknown: the meaning of record floats +172/+176 and of the word at object + 1328.
extern "C" {
extern int nmPlayers;
extern int gameMultiPlayerMarkersOn;
extern int frameNm;
extern void* haloTexture5;
extern unsigned char player[];
struct Point3 { float x, y, z; };
int netGetCameraCustomControl__Fi(int index);
int DLG_Active__Fv();
float isin__Fi(int angle);
float icos__Fi(int angle);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 position, float size, float depth, int a, int b, int c, int alpha, void* texture,
                                               short flags, int mode);

void maybeDrawMultiPlayerMarkers__6Player(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (*reinterpret_cast<int*>(object + 1328) != 0) return;
    if (nmPlayers == 1) return;
    if (gameMultiPlayerMarkersOn == 0) return;
    if (netGetCameraCustomControl__Fi(*reinterpret_cast<int*>(object + 960)) != 0 && (*reinterpret_cast<unsigned*>(object + 12) & 0x400000u) == 0) return;
    if (DLG_Active__Fv() != 0) return;
    const int index = *reinterpret_cast<int*>(object + 960);
    const unsigned char* record = player + index * 23984;
    const float health = *reinterpret_cast<const float*>(record + 172);
    if (health <= 0.0f) return;
    const float wave = isin__Fi((index * 500 + frameNm % 65536) * 562);
    const float radius = wave + 20.0f;
    const int alpha = static_cast<int>(wave * 14.0f + 57.0f);
    const float ratio = health / *reinterpret_cast<const float*>(record + 176);
    void* texture = haloTexture5;
    int a, b, c;
    if (ratio < 0.3333f && (frameNm & 2) != 0) {
        a = b = c = 0;
    } else if (index == 0) {
        a = 182; b = 47; c = 58;
    } else if (index == 1) {
        a = 24; b = 217; c = 30;
    } else if (index == 2) {
        a = 216; b = 219; c = 33;
    } else {
        a = 71; b = 61; c = 209;
    }
    const Point3* position = reinterpret_cast<const Point3*>(object + 16);
    for (int s = 0; s < 360; s += 8) {
        const int angle = static_cast<short>(static_cast<int>(static_cast<float>(s) * 182.04443359375f));
        const float dx = radius * isin__Fi(angle);
        const float dy = radius * icos__Fi(angle);
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3{position->x + dx, position->y + dy, position->z + 0.0f}, 6.0f, 1822.0f, a, b, c,
                                                  alpha, texture, 0, 0);
    }
}
}
