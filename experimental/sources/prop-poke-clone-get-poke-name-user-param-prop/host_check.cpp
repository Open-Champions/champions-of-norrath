#define NORRATH_VSLOTS_B_HOST 1
#include "msg_get_poke_name.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned frameNm = 0x12345678u;
static int failures = 0;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static char found_value[] = "hello";
static bool has_tag = false;
static int find_calls = 0;
static bool wrong_tag = false;
static unsigned short text_result[4] = {1, 2, 3, 0};
static char* text_key = nullptr;
static unsigned short* copy_dest = nullptr;
static unsigned short* copy_source = nullptr;
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence) {
    (void)object;
    ++find_calls;
    if (std::strcmp(tag, "message") != 0 || occurrence != 0) wrong_tag = true;
    return has_tag ? found_value : nullptr;
}
extern "C" unsigned short* textGetString__FPc(char* key) { text_key = key; return text_result; }
extern "C" void ustrcpy__FPUsT0(unsigned short* destination, unsigned short* source) { copy_dest = destination; copy_source = source; }
int main() {
    unsigned char object[400];
    unsigned short buffer[8] = {};
    std::memset(object, 0x44, sizeof object);
    has_tag = false;
    expect(msg_getPokeName__13UserParamPropPUsi(object, buffer, 5) == 0, "no tag returns zero");
    expect(vslots_b::read_word(vslots_b::address(object) + 360u) == 0x12345678u, "frameNm stored without a tag");
    expect(copy_dest == nullptr && text_key == nullptr && find_calls == 1, "no text work without a tag");
    expect(object[364] == 0x44 && object[359] == 0x44, "neighbours untouched");
    has_tag = true;
    frameNm = 0xcafef00du;
    find_calls = 0;
    expect(msg_getPokeName__13UserParamPropPUsi(object, buffer, 5) == 1, "tag returns one");
    expect(vslots_b::read_word(vslots_b::address(object) + 360u) == 0xcafef00du, "frameNm stored again");
    expect(text_key == found_value, "tag value is the text key");
    expect(copy_dest == buffer && copy_source == text_result, "text copied to the buffer");
    expect(find_calls == 2 && !wrong_tag, "two lookups of message at 0");
    return failures ? 1 : 0;
}
