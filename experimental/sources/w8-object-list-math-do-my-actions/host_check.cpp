// Host contract for objectDoMyActions__FP10GameObjecti. Not an EE plan input.
// Build: c++ -std=c++17 do_my_actions.cpp host_check.cpp
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "../w8-object-list-math-common/object_types.hpp"

struct ObjectWithTags {
    void* word0; ListNode* run_node; ListNode* grid_node; unsigned flags;
    unsigned char gap_to_44[44 - 16]; char** tags;
};
extern "C" void objectDoMyActions__FP10GameObjecti(ObjectWithTags*, int);
extern "C" ObjectWithTags* currentRunningObject = nullptr;
extern "C" unsigned char norrath_factory_ctype_minimum_view[384];

static std::vector<std::string> started;
static std::vector<ObjectWithTags*> running_at_start;
static void (*on_start)(const std::string&);
extern "C" void scriptStart__FPc(char* script) {
    started.push_back(script);
    running_at_start.push_back(currentRunningObject);
    if (on_start) on_start(started.back());
}
unsigned char norrath_factory_ctype_minimum_view[384];

static ObjectWithTags object;
static char* tags_storage[16];
static char text_hide[] = "hide", text_a[] = "action=a", text_name[] = "name=x", text_b[] = "action=b", text_upper[] = "ACTION=c",
            text_colon[] = "action:d", text_e[] = "action=e", text_short[] = "act", text_empty_value[] = "action=";

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    for (int c = 'A'; c <= 'Z'; ++c) norrath_factory_ctype_minimum_view[c + 128] = 1;

    char* list[] = {text_hide, text_a, text_name, text_b, text_upper, text_colon, text_e, nullptr};
    std::memcpy(tags_storage, list, sizeof list);
    object.tags = tags_storage;
    currentRunningObject = nullptr;

    objectDoMyActions__FP10GameObjecti(&object, 1);
    expect(started.size() == 3 && started[0] == "a" && started[1] == "b" && started[2] == "e", "actions run in tag order: text after 'action='");
    expect(running_at_start.size() == 3 && running_at_start[0] == &object && running_at_start[2] == &object, "currentRunningObject is the object during each script");
    expect(currentRunningObject == nullptr, "currentRunningObject cleared on exit");

    started.clear(); running_at_start.clear();
    objectDoMyActions__FP10GameObjecti(&object, 0);
    expect(started.size() == 3 && started[0] == "nota" && started[1] == "notb" && started[2] == "note", "a zero flag starts \"not\" + text");

    // The tag array is rescanned after each script: removing the first counted tag skips the next one.
    started.clear();
    static void (*remove_first)(const std::string&) = [](const std::string& script) {
        if (script == "a") { std::memmove(&tags_storage[1], &tags_storage[2], sizeof(char*) * 7); }
    };
    on_start = remove_first;
    objectDoMyActions__FP10GameObjecti(&object, 1);
    on_start = nullptr;
    expect(started.size() == 2 && started[0] == "a" && started[1] == "e", "rescan by count: after a's tag is removed the second counting tag (b) is skipped");

    // Edge tags: too short, empty value.
    char* list2[] = {text_short, text_empty_value, nullptr};
    std::memcpy(tags_storage, list2, sizeof list2);
    started.clear();
    objectDoMyActions__FP10GameObjecti(&object, 1);
    expect(started.size() == 1 && started[0] == "", "an empty value after '=' still runs, with empty text");

    // No tags: nothing starts.
    started.clear();
    tags_storage[0] = nullptr;
    objectDoMyActions__FP10GameObjecti(&object, 1);
    expect(started.empty() && currentRunningObject == nullptr, "empty array");
    object.tags = nullptr;
    currentRunningObject = &object;
    objectDoMyActions__FP10GameObjecti(&object, 1);
    expect(started.empty() && currentRunningObject == nullptr, "null array");

    // An action that is not first in the list.
    char* list3[] = {text_hide, text_name, text_e, nullptr};
    std::memcpy(tags_storage, list3, sizeof list3);
    object.tags = tags_storage;
    started.clear();
    objectDoMyActions__FP10GameObjecti(&object, 1);
    expect(started.size() == 1 && started[0] == "e", "single later action");
    if (failures) return 1;
    std::printf("objectDoMyActions contract passed\n");
    return 0;
}
