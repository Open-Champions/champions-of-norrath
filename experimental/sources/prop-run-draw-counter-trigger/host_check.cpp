#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "msg_trigger.cpp"
using namespace vslots_b;
static int failures = 0, order_count = 0, order[8];
static int string_calls, int_calls, action_calls, action_value, string_occurrence;
static char dummy[4] = {1, 0, 0, 0};
static char* string_result;
static int count_result, on_result, off_result;
static int int_occurrence[8];
static char int_tags[8][16];
static char string_tag[16];
static void* action_object;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence) {
    (void)object; ++string_calls; order[order_count++] = 1; string_occurrence = occurrence;
    std::strncpy(string_tag, tag, 15);
    return string_result;
}
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence) {
    (void)object; order[order_count++] = 2;
    if (int_calls < 8) { std::strncpy(int_tags[int_calls], tag, 15); int_occurrence[int_calls] = occurrence; }
    ++int_calls;
    if (std::strcmp(tag, "count") == 0) return count_result;
    if (std::strcmp(tag, "onOnly") == 0) return on_result;
    if (std::strcmp(tag, "offOnly") == 0) return off_result;
    return -1000;
}
extern "C" void objectDoMyActions__FP10GameObjecti(void* object, int value) {
    ++action_calls; action_object = object; action_value = value; order[order_count++] = 3;
}
static unsigned char object[100];
static void fresh(unsigned counter) {
    std::memset(object, 0, sizeof object);
    write_word(address(object) + 76u, counter);
    string_calls = int_calls = action_calls = order_count = 0;
    string_result = nullptr; count_result = 0; on_result = off_result = 0;
}
static unsigned counter() { return read_word(address(object) + 76u); }
int main() {
    // No count tag: the limit is 1.
    fresh(0);
    msg_trigger__7Counteri(object, 1);
    expect(counter() == 1u && action_calls == 1 && action_object == object && action_value == 1, "increment reaching 1 acts with the argument");
    expect(string_calls == 1 && std::strcmp(string_tag, "count") == 0 && string_occurrence == 0, "count tag looked up once");
    expect(int_calls == 2 && std::strcmp(int_tags[0], "onOnly") == 0 && std::strcmp(int_tags[1], "offOnly") == 0 && int_occurrence[0] == 0 && int_occurrence[1] == 0, "no count read; onOnly then offOnly");
    expect(order_count == 4 && order[0] == 1 && order[1] == 2 && order[2] == 2 && order[3] == 3, "order");
    fresh(5);
    msg_trigger__7Counteri(object, 1);
    expect(counter() == 6u && action_calls == 0 && int_calls == 0, "increment off the limit does nothing more");
    fresh(1);
    msg_trigger__7Counteri(object, 0);
    expect(counter() == 0u && action_calls == 1 && action_value == 0, "decrement acts when the old word equals the limit");
    fresh(2);
    msg_trigger__7Counteri(object, 0);
    expect(counter() == 1u && action_calls == 0, "decrement to the limit does not act");
    fresh(0);
    msg_trigger__7Counteri(object, 0);
    expect(counter() == 0xffffffffu && action_calls == 0, "decrement wraps in 32 bits");
    fresh(0);
    msg_trigger__7Counteri(object, -7);
    expect(counter() == 1u && action_value == -7, "any nonzero argument increments and is passed through");

    // A count tag supplies the limit.
    fresh(2);
    string_result = dummy; count_result = 3;
    msg_trigger__7Counteri(object, 1);
    expect(counter() == 3u && action_calls == 1 && int_calls == 3 && std::strcmp(int_tags[0], "count") == 0 && int_occurrence[0] == 0, "limit from the count tag");
    expect(order_count == 5 && order[0] == 1 && order[1] == 2 && order[2] == 2 && order[3] == 2 && order[4] == 3, "order with a count tag");
    fresh(2);
    string_result = dummy; count_result = 2;
    msg_trigger__7Counteri(object, 1);
    expect(action_calls == 0, "limit 2 is not reached by 3");
    fresh(3);
    string_result = dummy; count_result = 3;
    msg_trigger__7Counteri(object, 0);
    expect(counter() == 2u && action_calls == 1, "decrement acts at the old value 3");
    fresh(0);
    string_result = dummy; count_result = 0;
    msg_trigger__7Counteri(object, 1);
    expect(action_calls == 0 && counter() == 1u, "a zero count tag is the limit, not 1");
    fresh(0);
    string_result = dummy; count_result = 0;
    msg_trigger__7Counteri(object, 0);
    expect(action_calls == 1 && counter() == 0xffffffffu, "zero limit met by the old word");

    // Argument selection.
    fresh(1); on_result = 5;
    msg_trigger__7Counteri(object, 0);
    expect(action_value == 1, "onOnly forces 1 even for argument 0");
    fresh(0); off_result = 5;
    msg_trigger__7Counteri(object, 1);
    expect(action_value == 0, "offOnly forces 0");
    fresh(0); on_result = 5; off_result = 5;
    msg_trigger__7Counteri(object, 1);
    expect(action_value == 0, "offOnly beats onOnly");
    fresh(0); on_result = -1;
    msg_trigger__7Counteri(object, 4);
    expect(action_value == 1, "a negative tag value is non-zero");
    return failures == 0 ? 0 : 1;
}
