#define NORRATH_VSLOTS_B_HOST 1
#include "msg_trigger.cpp"
#include <cstdio>
#include <cstring>
static int finds = 0;
static int mutexes = 0;
static int starts = 0;
static unsigned mutex_result = 0u;
static char* find_result = nullptr;
static char* started = nullptr;
static const char* seen_tag = nullptr;
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence) {
    (void)object;
    ++finds;
    seen_tag = tag;
    if (occurrence != 0) finds = -10;
    return find_result;
}
extern "C" unsigned DLG_SetMutex__Fv() {
    ++mutexes;
    return mutex_result;
}
extern "C" void scriptStart__FPc(char* script) {
    ++starts;
    started = script;
}
int main() {
    unsigned char object[900];
    char script[] = "scene";
    std::memset(object, 0, sizeof object);
    msg_trigger__8Creaturei(object, 4);
    if (finds != 0) return 1;
    vslots_b::write_word(vslots_b::address(object) + 892u, 1u);
    find_result = nullptr;
    msg_trigger__8Creaturei(object, 4);
    if (finds != 1 || mutexes != 0 || std::strcmp(seen_tag, "dialogScript") != 0) return 1;
    find_result = script;
    mutex_result = 1u;
    msg_trigger__8Creaturei(object, 9);
    if (mutexes != 1 || starts != 0) return 1;
    mutex_result = 0u;
    msg_trigger__8Creaturei(object, 9);
    if (starts != 1 || started != script || finds != 4) return 1;
    return 0;
}
