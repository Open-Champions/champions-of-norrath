#define NORRATH_COMBAT_REACTIONS_HOST 1
#include "death.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static char* script_answer = nullptr;
static char* drop_answer = nullptr;
static char log_text[64];
static unsigned log_length = 0;
static char seen_script[16];
static char seen_drop_name[16];
static unsigned char seen_point[12];
static void* seen_state = nullptr;
static void* seen_deleted = nullptr;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static void note(char c) { log_text[log_length++] = c; log_text[log_length] = 0; }
extern "C" char* norrath_actor_find_tag_string(void*, char* tag, int occurrence) {
    if (occurrence != 0) note('!');
    if (std::strcmp(tag, "deathScript") == 0) { note('a'); return script_answer; }
    if (std::strcmp(tag, "dropitem") == 0) { note('b'); return drop_answer; }
    note('?');
    return nullptr;
}
extern "C" void scriptStart__FPc(char* script) { note('s'); std::strcpy(seen_script, script); }
extern "C" void dropItem__4itemG6Point3Pc(const void* point, char* name) {
    note('d');
    std::memcpy(seen_point, point, 12);
    std::strcpy(seen_drop_name, name);
}
extern "C" void norrath_animation_set_to_end(void* state) { note('e'); seen_state = state; }
extern "C" unsigned norrath_object_add_to_delete_list(void* object) { note('x'); seen_deleted = object; return 1u; }
static void reset() { log_length = 0; log_text[0] = 0; seen_state = nullptr; seen_deleted = nullptr; }
int main() {
    unsigned char object[400];
    char script_text[] = "scr";
    char drop_text[] = "gem";
    std::memset(object, 0x11, sizeof object);
    for (unsigned i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(100 + i);

    reset();
    script_answer = nullptr; drop_answer = nullptr;
    combat_reactions::write_word(combat_reactions::address(object) + 12u, 0x5u);
    death__8Creature(object);
    expect(std::strcmp(log_text, "ab") == 0, "no tags and no bit 2 only look up both tags");
    expect(combat_reactions::read_word(combat_reactions::address(object) + 12u) == 0x5u, "flags stay when bit 2 is clear");

    reset();
    script_answer = script_text; drop_answer = drop_text;
    combat_reactions::write_word(combat_reactions::address(object) + 12u, 0x3u);
    death__8Creature(object);
    expect(std::strcmp(log_text, "asbdex") == 0, "script, then drop, then end and delete");
    expect(std::strcmp(seen_script, "scr") == 0 && std::strcmp(seen_drop_name, "gem") == 0, "tag text is passed on");
    expect(std::memcmp(seen_point, object + 16, 12) == 0, "point bytes at +16 are copied");
    expect(seen_state == object + 288 && seen_deleted == object, "animation at 288 and the object");
    expect(combat_reactions::read_word(combat_reactions::address(object) + 12u) == 0x4003u, "0x4000 is ORed in");

    reset();
    script_answer = nullptr; drop_answer = drop_text;
    combat_reactions::write_word(combat_reactions::address(object) + 12u, 0x0u);
    death__8Creature(object);
    expect(std::strcmp(log_text, "abd") == 0, "drop without script and without bit 2");
    return failures ? 1 : 0;
}
