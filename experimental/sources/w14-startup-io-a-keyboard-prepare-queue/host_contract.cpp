#include "prepare_queue.cpp"
#include <cstring>
#include <utility>
#include <vector>

int sFrDeadkey[4] = {94, 96, 126, 168};
int sUsDeadkeyCount = 4;

static std::vector<std::pair<int, int>> queued;
static std::vector<int> combine_args;
static int combine_result;

extern "C" unsigned char CombineDeadkey__10VIKeyboardUc(void*, unsigned char c) { combine_args.push_back(c); return static_cast<unsigned char>(combine_result); }
extern "C" void QueueKeyEvent__10VIKeyboardP12VIConsoleKey(void*, void* r) {
    const unsigned char* p = static_cast<const unsigned char*>(r);
    queued.push_back({p[0], p[1]});
}

int main() {
    typedef std::vector<std::pair<int, int>> Q;
    for (int count : {0, 1, 2, 4})
    for (int pending : {0, 94, 0x1ff, 0x100, 7})
    for (int last : {-1, 0, 7, 65, 94})
    for (int code : {0, 94, 96, 126, 168, 65, 200, 255})
    for (int dir : {0, 1, 2})
    for (int combined : {0, 0x44}) {
        alignas(8) int words[140];
        std::memset(words, 0x33, sizeof words);
        words[132] = pending;
        words[133] = last == -1 ? code : last;   // last == -1 means "equals the incoming code"
        unsigned char key[2] = {static_cast<unsigned char>(code), static_cast<unsigned char>(dir)};
        queued.clear(); combine_args.clear(); sUsDeadkeyCount = count; combine_result = combined;
        PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey(words, key);
        bool dead = false;
        for (int i = 0; i < count; ++i) if (sFrDeadkey[i] == code) dead = true;
        Q want;
        int want132 = pending, want133 = words[133];
        int original133 = last == -1 ? code : last;
        want133 = original133;
        if (dir != 0) {
            if (pending != 0) {
                want133 = code;
                if (combined) want = {{combined, 1}, {combined, 0}};
                else want = {{pending & 0xff, 1}, {pending & 0xff, 0}, {code, 1}, {code, 0}};
                want132 = 0;
                if (combine_args.size() != 1 || combine_args[0] != code) return 1;
            } else {
                if (dead) want132 = code;
                if (!dead || code == 0) want = {{code, dir}};
                if (!combine_args.empty()) return 2;
            }
        } else {
            if (!(dead && code != 0) && original133 != code) want = {{code, dir}};
            if (original133 == code) want133 = 0;
            if (!combine_args.empty()) return 3;
        }
        if (queued != want) return 4;
        if (words[132] != want132 || words[133] != want133) return 5;
        for (int i = 0; i < 132; ++i) if (words[i] != 0x33333333) return 6;
    }
    return 0;
}
