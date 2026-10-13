// Host check only; not a build input. Defines NORRATH_HOST_CONTRACT so the old buffer words
// translate to host blocks. Allocation sizes, delete order, record clearing, the reset words and
// the caret call are compared with a written-out trace and the resulting memory.
#define NORRATH_HOST_CONTRACT 1
#include "set_max_text_len.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char self_object[512], old_text[16], old_records[16];
static unsigned char heap[2][512];
static int heap_next;
static std::string trace;
static const unsigned TAG_TEXT = 0x1000, TAG_RECORDS = 0x2000;

extern "C" {
char* norrath_host_translate(unsigned address) {
    return address == TAG_TEXT ? reinterpret_cast<char*>(old_text) : address == TAG_RECORDS ? reinterpret_cast<char*>(old_records) : nullptr;
}
void* __builtin_vec_new(unsigned long size) {
    trace += "new:" + std::to_string(size) + ";";
    unsigned char* block = heap[heap_next++];
    std::memset(block, 0xee, 512);
    return block;
}
void __builtin_vec_delete(void* p) {
    trace += p == old_text ? "del:text;" : p == old_records ? "del:records;" : "del:?;";
}
void SetCaretOffset__9VIWndEditi(void* self, int offset) {
    trace += std::string("caret:") + (self == self_object ? "T" : "?") + "," + std::to_string(offset) + ";";
}
}

int main() {
    bool ok = true;
    for (unsigned text_word : {0u, TAG_TEXT}) {
        for (unsigned record_word : {0u, TAG_RECORDS}) {
            for (int length : {0, 1, 3, 9, -2}) {
                std::memset(self_object, 0x66, sizeof self_object);
                std::memcpy(self_object + 424, &text_word, 4);
                std::memcpy(self_object + 432, &record_word, 4);
                heap_next = 0;
                trace.clear();
                SetMaxTextLen__9VIWndEditi(self_object, length);
                std::string want;
                if (text_word) want += "del:text;";
                if (record_word) want += "del:records;";
                want += "new:" + std::to_string(static_cast<unsigned long>(length * 2)) + ";new:" +
                        std::to_string(static_cast<unsigned long>(length * 8)) + ";caret:T,0;";
                ok = ok && trace == want && heap_next == 2;
                int stored;
                std::memcpy(&stored, self_object + 428, 4);
                ok = ok && stored == length;
                unsigned w436, w400;
                std::memcpy(&w436, self_object + 436, 4);
                std::memcpy(&w400, self_object + 400, 4);
                ok = ok && w436 == 0 && w400 == 0;
                // The text block starts with a zero halfword; the rest keeps the fill byte.
                ok = ok && heap[0][0] == 0 && heap[0][1] == 0 && heap[0][2] == 0xee;
                // Every byte of each of the length 8-byte records is cleared; the rest keeps the fill.
                for (int i = 0; i < 40; ++i) {
                    const bool cleared = length > 0 && i < length * 8;
                    if (cleared ? heap[1][i] != 0 : heap[1][i] != 0xee) ok = false;
                }
            }
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
