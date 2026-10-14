// Observed objectDoMyActions__FP10GameObjecti at 0x142310, extent 392.
// Runs the "action=" tags of an object as scripts, in tag order. The object's
// word at +44 points to a zero-terminated array of tag strings. The routine
// sets currentRunningObject (0x483acc) to the object for its whole duration and
// clears it on exit. It repeats the following with an action counter n that
// starts at 0:
//   - If the tag array is null, or empty, stop.
//   - Lower-case, in place and through the ctype table, the six-character
//     writable string "action" (0x483af0). It is already lower case, so the
//     write is idempotent.
//   - Walk the tags; a tag counts when strncmp(tag, "action", 6) == 0 and the
//     character at index 6 is '='. The tag array word is re-read after every
//     strncmp. The (n+1)-th counting tag is the one chosen; its text after the
//     '=' (tag + 7) is the action. No chosen tag stops the routine.
//   - Second argument nonzero: scriptStart(action). Zero: sprintf(buffer,
//     "not%s", action) into a 160-byte stack buffer and scriptStart(buffer).
//   - n grows by one.
// Unknown: the intended meaning of the "not" variant beyond what the format
// string shows, and the other fields of the object.
#include "../w8-object-list-math-common/object_types.hpp"

struct ObjectWithTags {
    void* word0;
    ListNode* run_node;
    ListNode* grid_node;
    unsigned flags;
    unsigned char gap_to_44[44 - 16];
    char** tags;
};

#if defined(__mips__)
static_assert(__builtin_offsetof(ObjectWithTags, tags) == 44, "observed offset");
#endif

extern "C" {
void scriptStart__FPc(char* script);
int sprintf(char* destination, const char* format, ...);
int strncmp(const char* left, const char* right, __SIZE_TYPE__ count);
extern const unsigned char norrath_factory_ctype_minimum_view[];
extern ObjectWithTags* currentRunningObject;
}

namespace {
char action_prefix[] = "action";

void lower_case_in_place(char* text) {
    for (; *text != 0; ++text) {
        const int character = *text;
        *text = static_cast<char>((norrath_factory_ctype_minimum_view[character + 128] & 1u) != 0 ? character + 32 : character);
    }
}
}  // namespace

extern "C" void objectDoMyActions__FP10GameObjecti(ObjectWithTags* object, int run_positive) {
    currentRunningObject = object;
    for (int executed = 0;; ++executed) {
        char* action = nullptr;
        if (object->tags != nullptr) {
            lower_case_in_place(action_prefix);
            if (object->tags[0] != nullptr) {
                int skip = executed;
                for (int index = 0;; ++index) {
                    if (strncmp(object->tags[index], action_prefix, 6) == 0) {
                        char* tag = object->tags[index];
                        if (tag[6] == '=' && --skip == -1) {
                            action = tag + 7;
                            break;
                        }
                    }
                    if (object->tags[index + 1] == nullptr) break;
                }
            }
        }
        if (action == nullptr) break;
        if (run_positive) {
            scriptStart__FPc(action);
        } else {
            char script[160];
            sprintf(script, "not%s", action);
            scriptStart__FPc(script);
        }
    }
    currentRunningObject = nullptr;
}
