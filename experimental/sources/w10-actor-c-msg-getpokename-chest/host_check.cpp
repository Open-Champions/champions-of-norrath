#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" int msg_getPokeName__5ChestPUsi(void*, unsigned short*, int);
static int failures = 0;
static std::vector<std::string> events;
static unsigned short table_entry[4] = {0x1111, 0x2222, 0x3333, 0};
static unsigned short table_entry2[4] = {0x4444, 0x5555, 0, 0};
static unsigned short formatted_marker[4] = {0x7777, 0x8888, 0, 0};
static unsigned short* last_text;
extern "C" unsigned short* textGetString__FPc(const char* key) {
    events.push_back(std::string("text:") + key);
    return std::string(key) == "OpenChest" ? table_entry : table_entry2;
}
extern "C" void ustrcpy__FPUsT0(unsigned short* destination, const unsigned short* source) {
    events.push_back(std::string("copy:") + (source == table_entry ? "open" : source == table_entry2 ? "pick" : source == last_text ? "formatted" : "?"));
    (void)destination;
}
static float fa[8];
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* destination, const unsigned short* format, float a, float b,
                                         float c, float d, float e, float f, float g, float h) {
    events.push_back(std::string("format:") + (format == table_entry2 ? "pick" : "?"));
    float v[8] = {a, b, c, d, e, f, g, h};
    std::memcpy(fa, v, sizeof v);
    last_text = destination;
    destination[0] = 0x7777;
}
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
int main() {
    alignas(8) unsigned char object[200];
    unsigned short name[16];
    const short amounts[] = {-5, 0, 1, 25, 32767, -32768};
    for (unsigned word : {0u, 1u, 0x80000000u})
        for (short amount : amounts)
            for (int arg : {0, 3}) {
                std::memset(object, 0x44, sizeof object);
                std::memcpy(object + 132, &word, 4);
                std::memcpy(object + 76, &amount, 2);
                unsigned char before[200];
                std::memcpy(before, object, sizeof object);
                events.clear();
                last_text = nullptr;
                std::memset(fa, 0, sizeof fa);
                int r = msg_getPokeName__5ChestPUsi(object, name, arg);
                expect(r == 1, "returns 1");
                expect(std::memcmp(before, object, sizeof object) == 0, "object not written");
                if (word == 0) {
                    expect(events.size() == 2 && events[0] == "text:OpenChest" && events[1] == "copy:open", "no amount word: open text");
                } else if (amount > 0) {
                    expect(events.size() == 3 && events[0] == "text:PickUpGold" && events[1] == "format:pick" &&
                               events[2] == "copy:formatted",
                           "amount: format then copy");
                    events.resize(0);
                    expect(fa[0] == static_cast<float>(amount), "first float is the amount");
                    bool zeros = true;
                    for (int i = 1; i < 8; ++i) if (fa[i] != 0.0f) zeros = false;
                    expect(zeros, "remaining floats are zero");
                } else {
                    expect(events.empty(), "non-positive amount does nothing");
                }
            }
    return failures ? 1 : 0;
}
