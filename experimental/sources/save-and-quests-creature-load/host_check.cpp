#define NORRATH_SQ_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static std::vector<std::string> trace;
static int type_orc, type_other;
static const void* dynamic_type = &type_other;
static unsigned char object[1024];
static unsigned char table[8 * 5];
static char texts[3][8] = {"idle", "walk", "die"};
static char name_text[] = "orcName";
static char wanted_text[] = "die";
extern "C" {
const char* objectSaveGetFromStringPool__Fi(int index) {
    trace.push_back("pool" + std::to_string(index));
    if (index == 41) return name_text;
    if (index == 42) return wanted_text;
    return nullptr;
}
void objectRemoveFromRunList__FP10GameObject(void* o) { trace.push_back(o == object ? "remove" : "remove?"); }
void animInitState__FP14AnimationState(void* s) { trace.push_back(s == object + 288 ? "init" : "init?"); }
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int c) {
    trace.push_back(std::string("add") + (s == object + 288 ? "" : "?") + std::to_string(reinterpret_cast<unsigned long long>(h) & 0xff) + "," + std::to_string(c));
}
void animSetToEnd__FP14AnimationState(void* s) { trace.push_back(s == object + 288 ? "end" : "end?"); }
const void* __tf3Orc() { return &type_orc; }
int __eq__C9type_infoRC9type_info(const void* a, const void* b) { return a == b; }
void __throw_bad_typeid() { trace.push_back("badtypeid"); }
}
#include "creature_load.cpp"
using sq::read_word;
using sq::write_word;
static void slot200(void* o) { trace.push_back(o == object + 3 ? "v200" : "v200?"); }
static void slot192(void* o) { trace.push_back(o == object - 2 ? "v192" : "v192?"); }
static const void* type_slot() { return dynamic_type; }
static unsigned char vtable[256];
static unsigned char buf[64];
static void put_slot(unsigned off, int adj, void* fn) {
    vtable[off] = (unsigned char)adj;
    vtable[off + 1] = (unsigned char)(adj >> 8);
    write_word(vtable + off + 4, sq::pointer_bits(fn));
}
static void setup() {
    std::memset(object, 0, sizeof object);
    std::memset(table, 0, sizeof table);
    std::memset(vtable, 0, sizeof vtable);
    std::memset(buf, 0x77, sizeof buf);
    trace.clear();
    dynamic_type = &type_other;
    put_slot(4 - 4, 0, (void*)type_slot);
    put_slot(200, 3, (void*)slot200);
    put_slot(192, -2, (void*)slot192);
    write_word(object + 72, sq::pointer_bits(vtable));
    write_word(object + 36, sq::pointer_bits(table));
    // object+0..: the object address itself must be stable across calls
    write_word(buf, 0xcafef00du);
    write_word(buf + 4, 40u);   // health 40
    write_word(buf + 8, 41u);   // name pool index
}
static void entry(int i, char* text, unsigned header) {
    write_word(table + i * 8, text ? sq::pointer_bits(text) : 0);
    write_word(table + i * 8 + 4, header);
}
static std::string joined() { std::string s; for (auto& l : trace) s += l + " "; return s; }
int main() {
    // alive: health converts from integer, name resolved, no dead block
    setup();
    buf[12] = 0x5c; buf[13] = 0; buf[14] = 0; buf[15] = 0;
    int n = msg_load__8CreaturePc(object, (char*)buf);
    if (n != 16) return 1;
    if (read_word(object + 244) != 0xcafef00du || sq::read_single(object + 560) != 40.0f) return 2;
    if (read_word(object + 892) != sq::pointer_bits(name_text)) return 3;
    if (joined() != "pool41 v200 ") return 18;
    if (object[570] != 0x5c || read_word(object + 12) != 0 || object[40] != 0) return 4;
    // dead: bits, animation setup from the matching table entry, orc type byte
    setup();
    write_word(buf + 4, 0xfffffffdu);  // health -3
    write_word(buf + 12, 42u);         // dead word -> "die"
    buf[16] = 0x11; buf[17] = 0x0f; buf[18] = 0; buf[19] = 0;
    entry(0, texts[0], 100);
    entry(1, texts[1], 101);
    entry(2, texts[2], 102);
    dynamic_type = &type_orc;
    write_word(object + 12, 0x10000u);
    write_word(object + 160, 0x00010000u);
    write_word(object + 164, 0);
    n = msg_load__8CreaturePc(object, (char*)buf);
    if (n != 20) return 5;
    if (sq::read_single(object + 560) != -3.0f) return 6;
    if (read_word(object + 12) != 0x12009u || object[572] != 10 || object[573] != 0) return 7;
    if (object[40] != 70) return 8;
    if (read_word(object + 160) != 0x01c10000u + 0 || read_word(object + 164) != 1u) return 9;
    if (object[570] != 0x11) return 10;
    if (trace.size() != 8 || trace[7] != "v192" || trace[1] != "v200" || trace[0] != "pool41" || trace[2] != "remove" || trace[3] != "pool42" || trace[4] != "init" ||
        trace[5] != "add102,4" || trace[6] != "end") return 11;
    // dead, type is not the orc type: byte 40 untouched; no match: no animation calls
    setup();
    write_word(buf + 4, 0u);
    write_word(buf + 12, 42u);
    buf[16] = 0; buf[17] = 0;
    entry(0, texts[2], 99);       // entry 0 is never searched
    entry(1, texts[1], 101);
    object[40] = 5;
    n = msg_load__8CreaturePc(object, (char*)buf);
    if (object[40] != 5 || n != 20) return 12;
    for (auto& l : trace) if (l == "init" || l == "end") return 13;
    // dead with a null pool result: no table walk
    setup();
    write_word(buf + 4, 0u);
    write_word(buf + 12, 7u);
    entry(1, texts[2], 5);
    msg_load__8CreaturePc(object, (char*)buf);
    for (auto& l : trace) if (l == "init") return 14;
    // flag bit 0: extra words (single, then low byte), then trailing byte and mask
    setup();
    write_word(object + 160, 1u);
    write_word(buf + 12, (unsigned)-9);
    write_word(buf + 16, 0x1ffu);
    buf[20] = 0x22; buf[21] = 0x0;
    n = msg_load__8CreaturePc(object, (char*)buf);
    if (n != 24 || sq::read_single(object + 564) != -9.0f || object[76] != 0xff || object[570] != 0x22) return 15;
    if (read_word(object + 160) != 1u) return 16;
    std::puts("ok");
    return 0;
}
