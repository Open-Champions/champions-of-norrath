// Host check for doNamePad__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "do_name_pad.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
float otherHermiteTime;
unsigned char showNoNamePrompt;
int curPlayer, nameGridPos, namePos, nameGridW, nameGridCount, padAcceptButtonFontChar;
unsigned padAcceptButton = 0x40, padBackButton = 0x20;
unsigned char pad[88 * 4];
unsigned char fontGrid[32];
unsigned char playerSelectInfo[41 * 4];
UString hudFooterUni[8];
}
static std::vector<std::string> events;
extern "C" {
UString* textGetString__FPc(const char* key) { events.push_back(std::string("text:") + key); static UString t[2]; return t; }
void usprintf__FPUsT0ffffffff(UString*, UString*, float a, float, float, float, float, float, float, float) { events.push_back("us:" + std::to_string(a)); }
void hudShowMsg__FPUsN20iii(UString*, UString*, UString*, int alpha, int a, int b) { events.push_back("msg:" + std::to_string(alpha) + std::to_string(a) + std::to_string(b)); }
void* lumpFindResource__FPcT0(const char* lump, const char* name) { events.push_back(std::string("lump:") + lump + ":" + name); return nullptr; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float) { events.push_back("sound"); }
void setNewGridPosition__Fii(int cell, int snap) { events.push_back("grid:" + std::to_string(cell) + "," + std::to_string(snap)); }
}
// the host's names for the two library routines the source binds by label
extern "C" unsigned lengthOf(const char* text) { return static_cast<unsigned>(std::strlen(text)); }

static void setWord(int slot, int offset, unsigned value) { std::memcpy(pad + 88 * slot + offset, &value, 4); }
static void reset() {
    events.clear(); std::memset(pad, 0, sizeof pad); std::memset(playerSelectInfo, 0, sizeof playerSelectInfo);
    otherHermiteTime = 1.0f; showNoNamePrompt = 0; curPlayer = 1; nameGridPos = 0; namePos = 0; nameGridW = 10; nameGridCount = 45;
    for (int i = 0; i < 26; ++i) fontGrid[i] = static_cast<unsigned char>('a' + i);
}
static std::string lastGrid() {
    for (size_t i = events.size(); i-- > 0;) if (events[i].rfind("grid:", 0) == 0) return events[i];
    return "";
}
static int move(unsigned mask, int from) { reset(); nameGridPos = from; setWord(1, 16, mask); doNamePad__Fv(); return 0; }

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    // slide-in: not exactly 1.0 grows by a quarter and returns early
    reset(); otherHermiteTime = 0.5f;
    CHECK(doNamePad__Fv() == 0 && otherHermiteTime == 0.75f && events.empty());
    reset(); otherHermiteTime = 0.9f;
    doNamePad__Fv(); CHECK(otherHermiteTime == 1.0f);
    // warning prompt: footer, message, accept clears it
    reset(); showNoNamePrompt = 1; padAcceptButtonFontChar = 7; setWord(1, 12, 0x40);
    CHECK(doNamePad__Fv() == 0 && showNoNamePrompt == 0 && events[0] == "text:MemcardContinue" && events[1] == "us:7.000000" && events[4] == "msg:12800");
    reset(); showNoNamePrompt = 1;
    CHECK(doNamePad__Fv() == 0 && showNoNamePrompt == 1);
    // moves: right (0x802000), left (0x408000), down (0x204000), up (0x101000)
    move(0x2000, 3); CHECK(lastGrid() == "grid:4,0" && events[0] == "lump:fx.lmp:menu_main_toggle.anm");
    move(0x2000, 9); CHECK(lastGrid() == "grid:0,1");               // end of the row wraps to its start
    move(0x2000, 44); CHECK(lastGrid() == "grid:-2,1");             // past the last cell goes to the space button
    move(0x2000, -1); CHECK(lastGrid() == "grid:40,1");             // count - count % width
    move(0x8000, 5); CHECK(lastGrid() == "grid:4,0");
    move(0x8000, 10); CHECK(lastGrid() == "grid:19,1");             // column 0 wraps to the end of the row above... row 1 start + 9
    move(0x8000, -2); CHECK(lastGrid() == "grid:44,1");
    move(0x4000, 12); CHECK(lastGrid() == "grid:22,0");
    move(0x4000, 41); CHECK(lastGrid() == "grid:-19,1");            // below the last row, column 1: the cell minus six widths
    move(0x4000, 44); CHECK(lastGrid() == "grid:-2,0");             // column 4 to 7 goes to the space button
    move(0x4000, 39); CHECK(lastGrid() == "grid:-1,0");             // column 8 or more goes to the done button
    move(0x4000, -1); CHECK(lastGrid() == "grid:9,1");
    move(0x4000, -2); CHECK(lastGrid() == "grid:5,1");
    move(0x1000, 25); CHECK(lastGrid() == "grid:15,0");
    move(0x1000, 3); CHECK(lastGrid() == "grid:63,1");              // above the first row, column 0 to 3: plus six widths
    move(0x1000, 5); CHECK(lastGrid() == "grid:-2,1");
    move(0x1000, 9); CHECK(lastGrid() == "grid:-1,1");
    move(0x8000, -1); CHECK(lastGrid() == "grid:-2,0");
    move(0x1000, -2); CHECK(lastGrid() == "grid:36,0");             // count - width + 1
    move(0x1000, -1); CHECK(lastGrid() == "grid:40,0");             // count - width + 5
    // no move: the highlighted cell is passed on, with nothing highlighted (-3) it starts at 0
    move(0, 7); CHECK(lastGrid() == "grid:7,0" && events.size() == 1);
    move(0, -3); CHECK(lastGrid() == "grid:0,0");
    // letter case follows pad word +8 bit 8
    reset(); setWord(1, 8, 8); doNamePad__Fv(); CHECK(fontGrid[0] == 'A' && fontGrid[25] == 'Z');
    setWord(1, 8, 0); doNamePad__Fv(); CHECK(fontGrid[0] == 'a' && fontGrid[25] == 'z');
    // typing: accept appends the highlighted letter; the space button appends a space; the limit is 30
    reset(); nameGridPos = 2; setWord(1, 12, 0x40);
    CHECK(doNamePad__Fv() == 0 && std::string(reinterpret_cast<char*>(playerSelectInfo + 41)) == "c" && namePos == 1);
    reset(); nameGridPos = -2; namePos = 1; playerSelectInfo[41] = 'x'; setWord(1, 12, 0x40);
    CHECK(doNamePad__Fv() == 0 && std::string(reinterpret_cast<char*>(playerSelectInfo + 41)) == "x " && namePos == 2);
    reset(); nameGridPos = 2; namePos = 30; setWord(1, 12, 0x40);
    CHECK(doNamePad__Fv() == 0 && namePos == 30);
    // back space (pad +16 bit 0x80) removes the last letter
    reset(); namePos = 2; playerSelectInfo[41] = 'a'; playerSelectInfo[42] = 'b'; setWord(1, 16, 0x80);
    doNamePad__Fv(); CHECK(namePos == 1 && playerSelectInfo[42] == 0 && playerSelectInfo[41] == 'a' && events[1] == "lump:fx.lmp:menu_main_back.anm");
    // done button: a real name finishes (1), an empty or all-space name warns; back returns -1
    reset(); nameGridPos = -1; setWord(1, 12, 0x40); playerSelectInfo[41] = 'q';
    CHECK(doNamePad__Fv() == 1 && showNoNamePrompt == 0);
    reset(); nameGridPos = -1; setWord(1, 12, 0x40); playerSelectInfo[41] = ' '; playerSelectInfo[42] = ' ';
    CHECK(doNamePad__Fv() == 0 && showNoNamePrompt == 1);
    reset(); nameGridPos = -1; setWord(1, 12, 0x40);
    CHECK(doNamePad__Fv() == 0 && showNoNamePrompt == 1);
    reset(); setWord(1, 12, 0x20);
    CHECK(doNamePad__Fv() == -1);
    std::puts("ok");
    return 0;
}
