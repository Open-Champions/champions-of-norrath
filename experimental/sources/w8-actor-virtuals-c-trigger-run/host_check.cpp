#define NORRATH_W8C_HOST 1
#include "run.cpp"
#include "../w8-actor-virtuals-c-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
unsigned char player[95936];
unsigned char nmPlayers[4];
unsigned char activePlayer[4];
unsigned char* currentRunningObject = nullptr;
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
// scripted environment
static unsigned char self_[400], matching[200], plain[200], params[200], tables[2][64], self_table[256];
static unsigned char players[2][64];
static int type_a = 0, type_b = 0;
static void* list_answer[3];
static float seen_low[3], seen_high[3];
static int find_calls = 0, capacity = 0;
static int tag_answers[8], tag_calls = 0;
static char seen_tags[8][16];
static int actions_calls = 0, action_arg = -1;
static int script_calls = 0, script_answer = 0;
static char seen_script[64];
static void* seen_running = nullptr;
static unsigned seen_active_player = 0;
static int slot_answers[2], slot_calls = 0;
static void* slot_players[2];
static void* slot_self = nullptr;
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** out, int cap) {
    ++find_calls; capacity = cap;
    std::memcpy(seen_low, low, 12); std::memcpy(seen_high, high, 12);
    for (int i = 0; i < 3; ++i) out[i] = list_answer[i];
}
extern "C" void* __tf13TriggerParams() { return &type_a; }
extern "C" int __eq__C9type_infoRC9type_info(void* l, void* r) { return l == r; }
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence) {
    expect(occurrence == 0, "occurrence zero");
    std::strcpy(seen_tags[tag_calls], tag);
    (void)object;
    return tag_answers[tag_calls++];
}
extern "C" void objectDoMyActions__FP10GameObjecti(void* object, int on) { ++actions_calls; action_arg = on; expect(object == params, "actions on params"); }
extern "C" int scriptStart__FPc(char* name) {
    ++script_calls; std::strcpy(seen_script, name);
    seen_running = currentRunningObject; seen_active_player = w8c::read_word(w8c::address(activePlayer));
    return script_answer;
}
extern "C" int sprintf(char* out, const char* format, ...) {
    expect(std::strcmp(format, "not%s") == 0, "negation pattern");
    __builtin_va_list args;
    __builtin_va_start(args, format);
    const char* text = __builtin_va_arg(args, const char*);
    __builtin_va_end(args);
    std::strcpy(out, "not");
    std::strcat(out, text);
    return static_cast<int>(std::strlen(out));
}
static void* type_a_fn() { return &type_a; }
static void* type_b_fn() { return &type_b; }
static int slot136(void* self, void* other) {
    slot_self = self;
    for (int i = 0; i < 2; ++i) if (other == slot_players[i]) { ++slot_calls; return slot_answers[i]; }
    return -1;
}

static void setup() {
    using namespace w8c;
    std::memset(self_, 0, sizeof self_); std::memset(matching, 0, sizeof matching); std::memset(plain, 0, sizeof plain);
    std::memset(params, 0, sizeof params); std::memset(self_table, 0, sizeof self_table);
    std::memset(tables, 0, sizeof tables); std::memset(player, 0, sizeof player);
    host_install_slot(self_, self_table, 136u, 8, reinterpret_cast<void*>(slot136));
    host_install_slot(matching, tables[0], 0u, 0, reinterpret_cast<void*>(type_a_fn));
    host_install_slot(plain, tables[1], 0u, 0, reinterpret_cast<void*>(type_b_fn));
    write_single(address(self_) + 76u, 1.0f); write_single(address(self_) + 80u, 2.0f);
    write_single(address(self_) + 84u, 3.0f); write_single(address(self_) + 88u, 4.0f);
    static char name[] = "door";
    host_note(name);
    write_word(address(self_) + 92u, address_bits(name));
    list_answer[0] = plain; list_answer[1] = matching; list_answer[2] = nullptr;
    self_[97] = 0;
    slot_players[0] = players[0]; slot_players[1] = players[1];
    host_note(players[0]); host_note(players[1]);
    write_word(address(player) + 21872u, address_bits(players[0]));
    write_word(address(player) + 21872u + 23984u, address_bits(players[1]));
    write_word(address(nmPlayers), 2u);
    slot_answers[0] = 0; slot_answers[1] = 0;
    find_calls = tag_calls = actions_calls = script_calls = slot_calls = 0;
    script_answer = 0; currentRunningObject = nullptr;
    std::memset(tag_answers, 0, sizeof tag_answers);
}
int main() {
    using namespace w8c;
    // first call: box search, match stored both ways; no players -> nothing else (state 0 == byte 96)
    setup();
    msg_run__7Trigger(self_);
    expect(find_calls == 1 && capacity == 128, "box search once with capacity 128");
    expect(seen_low[0] == 1.0f && seen_low[1] == 2.0f && seen_low[2] == -12000.0f, "low corner");
    expect(seen_high[0] == 3.0f && seen_high[1] == 4.0f && seen_high[2] == 12000.0f, "high corner");
    expect(read_word(address(matching) + 76u) == address_bits(self_) && read_word(address(self_) + 108u) == address_bits(matching), "match linked both ways");
    expect(read_word(address(plain) + 76u) == 0u, "non-matching object untouched");
    expect(self_[97] == 1 && read_word(address(self_) + 104u) == 0xffffffffu && self_[96] == 0 && script_calls == 0, "idle pass");
    msg_run__7Trigger(self_);
    expect(find_calls == 1, "no second search");
    // countdown
    setup(); self_[97] = 1; write_word(address(self_) + 100u, 3u);
    msg_run__7Trigger(self_);
    expect(read_word(address(self_) + 100u) == 2u && read_word(address(self_) + 104u) == 0u && script_calls == 0 && slot_calls == 0, "countdown only");
    // trigger by second player, no params object: script started, active player stored
    setup(); self_[97] = 1; slot_answers[0] = 0; slot_answers[1] = 1;
    msg_run__7Trigger(self_);
    expect(slot_calls == 2 && slot_self == self_ + 8, "both players asked, adjusted self");
    expect(script_calls == 1 && std::strcmp(seen_script, "door") == 0 && seen_running == self_ && seen_active_player == 1u, "script started for player 1");
    expect(currentRunningObject == nullptr && self_[96] == 1 && read_word(address(self_) + 104u) == 1u, "state and index stored");
    // already in that state: nothing
    msg_run__7Trigger(self_);
    expect(script_calls == 1, "unchanged state ends the call");
    // release without params: negated script name
    slot_answers[1] = 0;
    msg_run__7Trigger(self_);
    expect(script_calls == 2 && std::strcmp(seen_script, "notdoor") == 0 && self_[96] == 0, "negated name on release");
    // params object, timeout tag: onOnly=0, offOnly=0, then timeout=3 -> found stays 0, so nothing more
    setup(); self_[97] = 1; slot_answers[0] = 1; write_word(address(self_) + 108u, address_bits(params)); host_note(params);
    tag_answers[0] = 0; tag_answers[1] = 0;
    msg_run__7Trigger(self_);
    expect(tag_calls == 2 && std::strcmp(seen_tags[0], "onOnly") == 0 && std::strcmp(seen_tags[1], "offOnly") == 0, "two tag reads");
    expect(actions_calls == 1 && action_arg == 1 && script_calls == 1 && self_[96] == 1 && read_word(address(self_) + 100u) == 0u, "no tag found keeps the countdown");
    // offOnly found on press: decision 0, negated script, then timeout 3 -> countdown 12
    setup(); self_[97] = 1; slot_answers[0] = 1; write_word(address(self_) + 108u, address_bits(params)); host_note(params);
    tag_answers[0] = 0; tag_answers[1] = 1; tag_answers[2] = 3; tag_answers[3] = 3;
    msg_run__7Trigger(self_);
    expect(action_arg == 0 && std::strcmp(seen_script, "notdoor") == 0, "offOnly decides off");
    expect(std::strcmp(seen_tags[2], "timeout") == 0 && read_word(address(self_) + 100u) == 12u && self_[96] == 0, "timeout shifted by two");
    // onOnly found, timeout zero, needScriptHit set and script returned 0 -> countdown unchanged
    setup(); self_[97] = 1; slot_answers[0] = 1; write_word(address(self_) + 108u, address_bits(params)); host_note(params);
    tag_answers[0] = 1; tag_answers[1] = 0; tag_answers[2] = 0; tag_answers[3] = 1; script_answer = 0;
    msg_run__7Trigger(self_);
    expect(std::strcmp(seen_tags[3], "needScriptHit") == 0 && read_word(address(self_) + 100u) == 0u && self_[96] == 0, "needScriptHit with zero script result");
    // same but script returned nonzero -> countdown -1
    setup(); self_[97] = 1; slot_answers[0] = 1; write_word(address(self_) + 108u, address_bits(params)); host_note(params);
    tag_answers[0] = 1; tag_answers[1] = 0; tag_answers[2] = 0; tag_answers[3] = 1; script_answer = 7;
    msg_run__7Trigger(self_);
    expect(read_word(address(self_) + 100u) == 0xffffffffu && self_[96] == 0, "countdown -1");
    // timeout with countdown already -1 keeps it
    setup(); self_[97] = 1; slot_answers[0] = 1; write_word(address(self_) + 108u, address_bits(params)); host_note(params);
    tag_answers[0] = 1; tag_answers[2] = 5; tag_answers[3] = 5;
    msg_run__7Trigger(self_);
    expect(read_word(address(self_) + 100u) == 20u, "timeout 5 shifted");
    return failures ? 1 : 0;
}
