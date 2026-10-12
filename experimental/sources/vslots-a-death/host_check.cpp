#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void death__8Creature(void* self);

static std::vector<std::string> calls;
static unsigned char creature[320];
static const char* script_text = nullptr;
static const char* drop_text = nullptr;
static unsigned char dropped[12];
static int failures = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}

extern "C" char* findTagString__10GameObjectPci(void* object, char* tag, int index) {
    expect(object == creature, "tag lookup uses the creature");
    expect(index == 0, "tag index is zero");
    if (std::strcmp(tag, "deathScript") == 0) {
        calls.push_back("find-script");
        return const_cast<char*>(script_text);
    }
    if (std::strcmp(tag, "dropitem") == 0) {
        calls.push_back("find-drop");
        return const_cast<char*>(drop_text);
    }
    calls.push_back("find-other");
    return nullptr;
}
extern "C" int scriptStart__FPc(char* name) {
    calls.push_back(std::string("script:") + name);
    return 7;
}
extern "C" void dropItem__4itemG6Point3Pc(const void* point, char* name) {
    calls.push_back(std::string("drop:") + name);
    std::memcpy(dropped, point, 12);
}
static unsigned flags_at_animation = 0;
extern "C" void animSetToEnd__FP14AnimationState(void* state) {
    calls.push_back(state == creature + 288 ? "anim" : "anim-other");
    flags_at_animation = read_word(creature + 12);
}
extern "C" void objectAddToDeleteList__FP10GameObject(void* object) {
    calls.push_back(object == creature ? "delete" : "delete-other");
}

static void reset(unsigned flags) {
    calls.clear();
    std::memset(creature, 0x5a, sizeof creature);
    for (unsigned i = 0; i < 12u; ++i) creature[16 + i] = static_cast<unsigned char>(0x30 + i);
    write_word(creature + 12, flags);
    std::memset(dropped, 0, sizeof dropped);
}

int main() {
    script_text = nullptr;
    drop_text = nullptr;
    reset(0);
    death__8Creature(creature);
    expect(calls.size() == 2 && calls[0] == "find-script" && calls[1] == "find-drop", "missing tags stop after the lookups");
    expect(read_word(creature + 12) == 0u, "flag 0 leaves the flags word");

    script_text = "dies";
    drop_text = "coin";
    reset(1);
    death__8Creature(creature);
    expect(calls.size() == 4 && calls[1] == "script:dies" && calls[3] == "drop:coin", "tags start the script and the drop");
    bool point = true;
    for (unsigned i = 0; i < 12u; ++i) if (dropped[i] != creature[16 + i]) point = false;
    expect(point, "drop copies the 12 bytes at creature+16");
    expect(read_word(creature + 12) == 1u, "flag bit 0 does not add the delete bit");

    script_text = nullptr;
    drop_text = nullptr;
    reset(2);
    death__8Creature(creature);
    expect(calls.size() == 4 && calls[2] == "anim" && calls[3] == "delete", "flag bit 2 ends the animation and deletes");
    expect(flags_at_animation == 0x4002u && read_word(creature + 12) == 0x4002u, "flag bit 2 sets bit 0x4000 before the animation");

    script_text = "dies";
    drop_text = "coin";
    reset(0x4006);
    death__8Creature(creature);
    expect(calls.size() == 6 && calls[1] == "script:dies" && calls[3] == "drop:coin" && calls[4] == "anim" && calls[5] == "delete",
           "tags run before the delete path");
    expect(flags_at_animation == 0x4006u && read_word(creature + 12) == 0x4006u, "an existing 0x4000 bit stays set");
    return failures ? 1 : 0;
}
