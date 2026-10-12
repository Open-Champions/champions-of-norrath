#include <cstring>
#include <vector>
signed char netNetworkMode;
int chatOn, frontEndRunning, waiting;
unsigned char outMsg[160];
signed char inMsg[8][126];
int msgTimeout[8];
static std::vector<int> raw, translated;
static int raw_calls, sends, localPlayer = 3;
extern "C" int keyboardGetNextKey__Fv() { raw_calls++; if (raw.empty()) return -1; int k = raw.front(); raw.erase(raw.begin()); return k; }
extern "C" int keyboardGetNextTranslatedKey__Fv() { if (translated.empty()) return -1; int k = translated.front(); translated.erase(translated.begin()); return k; }
extern "C" int netGetLocalPlayerNmSafe__Fi(int i) { return i == 0 ? localPlayer : 99; }
extern "C" void textSend__Fv() { sends++; }
extern "C" void textMessageProcess__Fv();

static void reset() {
    memset(outMsg, 0, sizeof outMsg);
    outMsg[0] = 1; outMsg[3] = 32;
    raw.clear(); translated.clear(); raw_calls = sends = 0;
    netNetworkMode = 1; chatOn = 1; frontEndRunning = 0; waiting = 0; localPlayer = 3;
    memset(inMsg, 0, sizeof inMsg); memset(msgTimeout, 0, sizeof msgTimeout);
    for (int i = 0; i < 8; i++) inMsg[i][2] = -2;
}
static void type(const char* s) { for (; *s; s++) translated.push_back((unsigned char)*s); }

int main() {
    // Disabled chat or network drains the raw key queue and does nothing else.
    reset(); netNetworkMode = 0; raw = {1, 2, 3}; translated = {'a'};
    textMessageProcess__Fv();
    if (!raw.empty() || raw_calls != 4 || outMsg[1] != 0 || outMsg[2] != 0 || translated.size() != 1) return 1;
    reset(); chatOn = 0; raw = {5}; textMessageProcess__Fv();
    if (!raw.empty() || outMsg[2] != 0) return 2;
    // Front end running: untouched.
    reset(); frontEndRunning = 1; raw = {5}; translated = {'a'}; textMessageProcess__Fv();
    if (raw.size() != 1 || translated.size() != 1 || outMsg[2] != 0) return 3;
    // Timeouts count the entries that are not 0xfe; waiting stops key handling after that.
    reset(); inMsg[1][2] = 5; inMsg[6][2] = 0; waiting = 1; translated = {'a'};
    textMessageProcess__Fv();
    for (int i = 0; i < 8; i++) if (msgTimeout[i] != (i == 1 || i == 6 ? 1 : 0)) return 4;
    if (outMsg[2] != 3 || translated.size() != 1 || outMsg[1] != 0) return 5;
    // Typing: "abc" gives cells a b c then the original space.
    reset(); type("abc"); textMessageProcess__Fv();
    if (outMsg[0] != 4 || outMsg[1] != 3 || memcmp(outMsg + 3, "abc ", 4)) return 6;
    // Left twice, insert X at the cursor.
    reset(); type("abc"); translated.push_back(17); translated.push_back(17); translated.push_back('X');
    textMessageProcess__Fv();
    if (outMsg[0] != 5 || outMsg[1] != 2) return 7;
    // Left copies the cell to its left onto the cursor cell, so "abc " becomes "abcc".
    reset(); type("abc"); translated.push_back(17);
    textMessageProcess__Fv();
    if (outMsg[1] != 2 || memcmp(outMsg + 3, "abcc", 4) != 0) return 8;
    // Right at the end stays put; left at the start stays put.
    reset(); translated = {17, 18}; textMessageProcess__Fv();
    if (outMsg[1] != 0 || outMsg[0] != 1) return 10;
    // Backspace removes the cell before the cursor: type "ab", backspace.
    reset(); type("ab"); translated.push_back(8); textMessageProcess__Fv();
    if (outMsg[0] != 2 || outMsg[1] != 1 || memcmp(outMsg + 3, "a ", 2)) return 11;
    // Backspace at the start does nothing.
    reset(); translated = {8}; textMessageProcess__Fv();
    if (outMsg[0] != 1 || outMsg[1] != 0) return 12;
    // Delete: type "ab", left, left (pos 0), delete removes the cell under the cursor.
    reset(); type("ab"); translated.push_back(17); translated.push_back(17); translated.push_back(26);
    textMessageProcess__Fv();
    if (outMsg[0] != 2 || outMsg[1] != 0) return 13;
    // Right moves forward when not on the last cell.
    reset(); type("ab"); translated.push_back(17); translated.push_back(17); translated.push_back(18);
    textMessageProcess__Fv();
    if (outMsg[1] != 1) return 14;
    // Delete on the last cell does nothing.
    reset(); translated = {26}; textMessageProcess__Fv();
    if (outMsg[0] != 1) return 15;
    // Enter sends and stops reading keys; with player index 0xff nothing is sent.
    reset(); translated = {13, 'z'}; textMessageProcess__Fv();
    if (sends != 1 || translated.size() != 1) return 16;
    reset(); localPlayer = 0xff; translated = {13, 'z'}; textMessageProcess__Fv();
    if (sends != 0 || outMsg[0] != 2) return 17;
    // The length limit: no insertion once len reaches 122.
    reset(); outMsg[0] = 122; translated = {'q'}; textMessageProcess__Fv();
    if (outMsg[0] != 122 || outMsg[1] != 0) return 18;
    reset(); outMsg[0] = 121; translated = {0x141}; textMessageProcess__Fv();
    if (outMsg[0] != 122 || outMsg[1] != 1 || outMsg[3] != 0x41) return 19;
    return 0;
}
