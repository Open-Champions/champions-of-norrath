// Host contract for lightSetupForWorld__FP6PacketP13PolysetHeader. Not an EE plan input.
// findLightsInBox_forWorld is a stub that returns a prepared list. Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
struct Box { float v[6]; };
struct Packet { unsigned char* cursor; unsigned char* last; };
static const unsigned char* prepared[32];
static int preparedCount;
static Box seenBox; static int seenMax;
extern "C" {
void findLightsInBox_forWorld__FG3BoxPP5Lighti(Box box, const unsigned char** out, int max) {
    seenBox = box; seenMax = max;
    for (int i = 0; i < preparedCount; ++i) out[i] = prepared[i];
    if (preparedCount < 31) out[preparedCount] = nullptr;
}
int lightSetupForWorld__FP6PacketP13PolysetHeader(Packet*, const unsigned char*);
}
static void setf(unsigned char* b, int off, float v) { std::memcpy(b + off, &v, 4); }
static void seth(unsigned char* b, int off, unsigned short v) { std::memcpy(b + off, &v, 2); }
static void setw(unsigned char* b, int off, unsigned v) { std::memcpy(b + off, &v, 4); }
static float getf(const unsigned char* b, int off) { float v; std::memcpy(&v, b + off, 4); return v; }
static unsigned getw(const unsigned char* b, int off) { unsigned v; std::memcpy(&v, b + off, 4); return v; }
static unsigned short geth(const unsigned char* b, int off) { unsigned short v; std::memcpy(&v, b + off, 2); return v; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    alignas(16) unsigned char header[128] = {};
    float corners[6] = {0, 0, 0, 8, 8, 8};
    std::memcpy(header + 8, corners, 24);
    alignas(16) unsigned char lightA[96] = {}, lightB[96] = {};
    alignas(16) unsigned char buffer[400];
    std::memset(buffer, 0xaa, sizeof buffer);
    Packet packet{buffer + 16, nullptr};

    // No lights: every slot is the default, and the result is 0.
    preparedCount = 0;
    int r = lightSetupForWorld__FP6PacketP13PolysetHeader(&packet, header);
    const unsigned char* base = buffer + 20;
    expect(r == 0, "no lights returns 0");
    expect(packet.cursor == base + 140 && seenMax == 32, "cursor advance and list size");
    expect(seenBox.v[3] == 8.0f && seenBox.v[0] == 0.0f, "box copy passed");
    expect(getw(buffer, 16) == 0x6d048000u && getw(base, 32) == 0x1400000eu && getw(base, 36) == 0x6c06c000u && getw(base, 136) == 0x14000004u, "tag words");
    for (int s = 0; s < 4; ++s) {
        expect(geth(base, 8 * s) == 0 && geth(base, 8 * s + 6) == 0, "default quad");
        expect(getf(base, 40 + 4 * s) == 1.0f && getw(base, 56 + 4 * s) == 0 && getw(base, 72 + 4 * s) == 0, "default rows");
    }

    // Two lights, bit 0 of word 36 set. A is farther than B; B has intensity 2 (high slot).
    setw(header, 36, 1);
    setw(header, 52, (1u << 18) | 0x10000u | 0x20000u);
    setf(lightA, 44, 0.5f); setf(lightA, 48, 4.0f); setf(lightA, 52, 8.0f); setf(lightA, 56, 12.0f);
    setf(lightA, 36, 3.0f); setf(lightA, 40, 0.25f);
    for (int i = 0; i < 4; ++i) seth(lightA, 18 + 2 * i, static_cast<unsigned short>(100 + i));
    setf(lightB, 44, 2.0f); setf(lightB, 48, 4.0f); setf(lightB, 52, 4.0f); setf(lightB, 56, 4.0f);
    for (int i = 0; i < 4; ++i) seth(lightB, 18 + 2 * i, static_cast<unsigned short>(200 + i));
    prepared[0] = lightA; prepared[1] = lightB; preparedCount = 2;
    std::memset(buffer, 0xaa, sizeof buffer);
    packet.cursor = buffer + 16;
    r = lightSetupForWorld__FP6PacketP13PolysetHeader(&packet, header);
    expect(r == 1, "lights placed returns 1");
    // B (center distance 0) is chosen first; its intensity >= 1 takes slot 3. A takes slot 0.
    expect(geth(base, 24) == 200 && geth(base, 30) == 203, "B quad in slot 3");
    expect(geth(base, 0) == 100 && geth(base, 6) == 103, "A quad in slot 0");
    expect(getf(base, 40 + 12) == 2.0f && getf(base, 40) == 0.5f, "row 0 is the intensity");
    // axis = 1: a = py * 0.25, b = px * 0.25; both negated.
    expect(getf(base, 56) == -(8.0f * 0.25f) && getf(base, 72) == -(4.0f * 0.25f), "slot 0 rows 1 and 2");
    expect(getf(base, 56 + 12) == -(4.0f * 0.25f) && getf(base, 72 + 12) == -(4.0f * 0.25f), "slot 3 rows 1 and 2");
    expect(getf(base, 88) == 12.0f * 0.25f && getf(base, 104) == (3.0f * 3.0f) * 0.0625f && getf(base, 120) == -0.25f * 16.0f, "slot 0 rows 3 to 5");
    // Slots 1 and 2 are filled with the default.
    expect(getf(base, 44) == 1.0f && getf(base, 48) == 1.0f && getw(base, 60) == 0, "unused slots default");

    // Clear flag: rows 1 and 2 come from the two header shorts.
    setw(header, 36, 0);
    seth(header, 54, 32767); seth(header, 56, 0);
    prepared[0] = lightA; preparedCount = 1;
    packet.cursor = buffer + 16;
    lightSetupForWorld__FP6PacketP13PolysetHeader(&packet, header);
    const float k = 1.0f / 32767.0f;
    const float u = 32767 * k;
    expect(getf(base, 56) == ((u * 4.0f + 0.0f * 8.0f) * 0.25f), "row 1 from the shorts");
    expect(getf(base, 72) == (((-0.0f) * 4.0f + u * 8.0f) * 0.25f), "row 2 from the shorts");
    if (failures) return 1;
    std::printf("lightSetupForWorld contract passed\n");
    return 0;
}
