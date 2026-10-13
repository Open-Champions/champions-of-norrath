// Host check for playerAllocateArmorCache__Fii. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// The image's 32-bit pointer words are offsets into one host arena. The item table, group tables
// and racePrefix are small stand-ins built here; expected sizes are worked out by hand from the
// rules in the source header.
#define NORRATH_HOST_ARENA
#include "player_allocate_armor_cache.cpp"

#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>

static unsigned char arena[1 << 20];
extern "C" {
unsigned char* norrath_host_base = arena;
alignas(8) unsigned char norrath_player_armor_cache_view_7cb770[3520];
alignas(8) unsigned char _4item$itemTable[64 * 24];
int norrath_equipment_slot_to_cache[18] __asm__("equipmentSlot2Cache.1137") = {7, 8, 6, 9, 0, -1, -1, -1, -1, 2, 1, 3, 4, 5, -1, -1, -1, -1};
const char* norrath_armor_name[10] __asm__("armorName.1138") = {"n0", "n1", "n2", "n3", "n4", "n5", "n6", "n7", "n8", "n9"};
char* racePrefix[10] = {const_cast<char*>("bm"), const_cast<char*>("bf"), const_cast<char*>("wem"), const_cast<char*>("wef"), const_cast<char*>("hem"),
                        const_cast<char*>("hef"), const_cast<char*>("em"), const_cast<char*>("ef"), const_cast<char*>("dem"), const_cast<char*>("def")};
int nmWGroups, nmEqGroups, nmPlayers;
unsigned wGroup, eqGroup;
signed char coreForceSmallCharacters;
}

static std::string trace;
static int allocCount;
extern "C" void* memAlloc__FiPc(int size, char* label) {
    char b[80];
    std::snprintf(b, sizeof b, "alloc %d %s;", size, label);
    trace += b;
    return arena + 0x80000 + 0x1000 * allocCount++;
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

static unsigned bump;
static unsigned put(const char* s) {
    const unsigned at = bump;
    std::strcpy(reinterpret_cast<char*>(arena + at), s);
    bump += static_cast<unsigned>(std::strlen(s)) + 1;
    return at;
}
struct Row { const char* text; int kind; };
struct Group { const char* name; int count; std::vector<std::pair<int, int>> parts; };  // parts: (hw+2, hw+10) per 16 bytes

static void setRows(const std::vector<Row>& rows) {
    std::memset(_4item$itemTable, 0, sizeof _4item$itemTable);
    for (size_t i = 0; i < rows.size(); ++i) {
        unsigned char* r = _4item$itemTable + 64 * i;
        const unsigned nameWord = 0x100 + static_cast<unsigned>(i);
        std::memcpy(r, &nameWord, 4);
        r[28] = static_cast<unsigned char>(rows[i].kind);
        const unsigned textWord = rows[i].text ? put(rows[i].text) : 0;
        std::memcpy(r + 48, &textWord, 4);
    }
}
int main() {
    // build a world by hand with exact part records
    auto run = [&](int player, int race, const std::vector<Row>& rows, const std::vector<Group>& wgroups, const std::vector<Group>& egroups,
                   const unsigned char* flagRows) -> int {
        std::memset(arena, 0, 0x40000);
        bump = 0x100;
        trace.clear();
        allocCount = 0;
        std::memset(norrath_player_armor_cache_view_7cb770, 0x11, sizeof norrath_player_armor_cache_view_7cb770);
        for (int row = 0; row < 10; ++row)
            for (int q = 0; q < 4; ++q) norrath_player_armor_cache_view_7cb770[352 * row + 88 * q + 84] = flagRows[row];
        setRows(rows);
        auto build = [&](const std::vector<Group>& groups, unsigned& tableWord) {
            tableWord = bump;
            bump += 12 * static_cast<unsigned>(groups.size()) + 4;
            bump = (bump + 7) & ~7u;
            for (size_t i = 0; i < groups.size(); ++i) {
                const unsigned nameWord = put(groups[i].name);
                bump = (bump + 7) & ~7u;
                const unsigned parts = bump;
                bump += 16 * static_cast<unsigned>(groups[i].parts.size() + 2);
                for (size_t k = 0; k < groups[i].parts.size(); ++k) {
                    // each 16-byte step has two 8-byte records; the halfwords at +2 and +10 are used
                    const short first = static_cast<short>(groups[i].parts[k].first);
                    const short second = static_cast<short>(groups[i].parts[k].second);
                    std::memcpy(arena + parts + 16 * k + 2, &first, 2);
                    std::memcpy(arena + parts + 16 * k + 10, &second, 2);
                }
                unsigned char* g = arena + tableWord + 12 * i;
                std::memcpy(g, &parts, 4);
                std::memcpy(g + 4, &nameWord, 4);
                const int count = groups[i].count;
                std::memcpy(g + 8, &count, 4);
            }
        };
        unsigned w = 0, e = 0;
        build(wgroups, w);
        build(egroups, e);
        wGroup = w; eqGroup = e;
        nmWGroups = static_cast<int>(wgroups.size());
        nmEqGroups = static_cast<int>(egroups.size());
        playerAllocateArmorCache__Fii(player, race);
        return 0;
    };
    unsigned char noFlags[10] = {};
    unsigned char row6Flag[10] = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
    // Case A: race 0 (prefix "bm"), player 2. Rows: a plain armor piece, a "cloth" piece (skipped for
    // this race), a copied-as-is piece in cache row 6 (flag set: wGroup), a piece in rows 7 and 8,
    // a null text and an unmapped kind (both skipped).
    nmPlayers = 1; coreForceSmallCharacters = 0;
    std::vector<Row> rows = {{"plate7", 4}, {"cloth_robe1", 4}, {"helm12", 2}, {"sword", 0}, {"bow", 1}, {nullptr, 4}, {"unmapped", 5}, {"cloth2", 0}};
    std::vector<Group> eg = {{"bm_plate", 4, {{3, 4}, {1, 1}}}, {"sword", 2, {{2, 2}}}, {"bow", 2, {{1, 1}}}};
    std::vector<Group> wg = {{"helm", 2, {{5, 0}}}, {"bm_plate", 2, {{40, 40}}}};
    // bm_plate: eqGroup (row 0 flag 0): (3+4)<<11 = 14336; the second step (count 4: two steps) (1+1)<<11 = 4096 -> 14336.
    // helm (row 6, flag set -> wGroup): one step, 5<<11 = 10240.  sword (row 7): 4<<11 = 8192.  bow (row 8): 2<<11 = 4096 -> max(4096, sizes[7]) = 8192.
    run(2, 0, rows, wg, eg, row6Flag);
    CHECK(trace == "alloc 14336 n0;alloc 0 n1;alloc 0 n2;alloc 0 n3;alloc 0 n4;alloc 0 n5;alloc 10240 n6;alloc 8192 n7;alloc 8192 n8;alloc 0 n9;");
    for (int row = 0; row < 10; ++row) {
        unsigned char* el = norrath_player_armor_cache_view_7cb770 + 352 * row + 88 * 2;
        unsigned block, size;
        std::memcpy(&block, el, 4); std::memcpy(&size, el + 4, 4);
        CHECK(el[85] == 1 && el[86] == 0);
        const unsigned expectSize[10] = {14336, 0, 0, 0, 0, 0, 10240, 8192, 8192, 0};
        CHECK(size == expectSize[row]);
        CHECK(block == (static_cast<unsigned>(0x80000 + 0x1000 * row)));
        // other players' elements and the unused bytes of this one are untouched
        for (int q : {0, 1, 3}) CHECK(norrath_player_armor_cache_view_7cb770[352 * row + 88 * q + 85] == 0x11);
        CHECK(el[10] == 0x11 && el[84] == (row == 6 ? 1 : 0));
    }
    // Case B: race 6 (a "cloth" race, prefix "em"): only cloth texts are kept
    nmPlayers = 1;
    std::vector<Row> rowsB = {{"plate7", 4}, {"cloth_robe1", 4}, {"cloth2", 3}, {"boots", 2}};
    std::vector<Group> egB = {{"em_cloth_robe", 2, {{6, 0}}}, {"em_cloth", 2, {{2, 2}}}, {"boots", 2, {{3, 3}}}, {"em_plate", 2, {{9, 9}}}};
    run(0, 6, rowsB, {}, egB, noFlags);
    // cloth_robe1 -> "em_cloth_robe" -> row 0: 6<<11 = 12288; cloth2 (kind 3 -> row 9, copied as is) -> "cloth" -> not in groups; boots (kind 2 -> row 6): 6<<11 = 12288
    CHECK(trace == "alloc 12288 n0;alloc 0 n1;alloc 0 n2;alloc 0 n3;alloc 0 n4;alloc 0 n5;alloc 12288 n6;alloc 0 n7;alloc 0 n8;alloc 0 n9;");
    {
        unsigned char* el = norrath_player_armor_cache_view_7cb770;
        CHECK(el[85] == 7 && el[86] == 0);
    }
    // race 7 is a cloth race too; race 5 is not (the bound is 6 <= race < 8)
    run(1, 7, rowsB, {}, {{"ef_cloth_robe", 2, {{1, 1}}}}, noFlags);
    CHECK(trace.find("alloc 4096 n0;") == 0);
    run(1, 5, rowsB, {}, {{"hef_plate", 2, {{1, 1}}}}, noFlags);
    CHECK(trace.find("alloc 4096 n0;") == 0);
    // the byte at +86: nmPlayers or coreForceSmallCharacters
    for (int players : {1, 2, 4})
        for (int small : {0, 1, -1}) {
            nmPlayers = players; coreForceSmallCharacters = static_cast<signed char>(small);
            run(3, 9, {}, {}, {}, noFlags);
            for (int row = 0; row < 10; ++row) {
                unsigned char* el = norrath_player_armor_cache_view_7cb770 + 352 * row + 88 * 3;
                CHECK(el[85] == 10 && el[86] == ((players >= 2 || small != 0) ? 1 : 0));
            }
        }
    nmPlayers = 1; coreForceSmallCharacters = 0;
    // an empty item table still allocates ten blocks of size zero
    run(0, 0, {}, {}, {}, noFlags);
    CHECK(trace == "alloc 0 n0;alloc 0 n1;alloc 0 n2;alloc 0 n3;alloc 0 n4;alloc 0 n5;alloc 0 n6;alloc 0 n7;alloc 0 n8;alloc 0 n9;");
    // a group with a zero or negative count is ignored
    run(0, 0, {{"plate", 4}}, {}, {{"bm_plate", 0, {{9, 9}}}}, noFlags);
    CHECK(trace.find("alloc 0 n0;") == 0);
    run(0, 0, {{"plate", 4}}, {}, {{"bm_plate", -2, {{9, 9}}}}, noFlags);
    CHECK(trace.find("alloc 0 n0;") == 0);
    // digit trimming removes every trailing digit and stops at a letter
    run(0, 0, {{"plate123", 4}}, {}, {{"bm_plate", 2, {{1, 1}}}}, noFlags);
    CHECK(trace.find("alloc 4096 n0;") == 0);
    run(0, 0, {{"pl9ate", 4}}, {}, {{"bm_pl9ate", 2, {{1, 1}}}}, noFlags);
    CHECK(trace.find("alloc 4096 n0;") == 0);
    // steps: count 6 walks three 16-byte steps, count 5 also three, count 4 two, count 2 one
    const std::vector<std::pair<int, int>> steps = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
    run(0, 0, {{"plate", 4}}, {}, {{"bm_plate", 6, steps}}, noFlags);
    CHECK(trace.find("alloc 12288 n0;") == 0);
    run(0, 0, {{"plate", 4}}, {}, {{"bm_plate", 5, steps}}, noFlags);
    CHECK(trace.find("alloc 12288 n0;") == 0);
    run(0, 0, {{"plate", 4}}, {}, {{"bm_plate", 4, steps}}, noFlags);
    CHECK(trace.find("alloc 8192 n0;") == 0);
    run(0, 0, {{"plate", 4}}, {}, {{"bm_plate", 2, steps}}, noFlags);
    CHECK(trace.find("alloc 4096 n0;") == 0);
    run(0, 0, {{"plate", 4}}, {}, {{"bm_plate", 4, {{9, 9}, {1, 1}}}}, noFlags);
    CHECK(trace.find("alloc 36864 n0;") == 0);    // the largest step wins, not the last
    // race 8 and 9 are plain races: cloth texts are skipped and the prefix comes from the race
    run(0, 8, {{"cloth_robe1", 4}, {"plate", 4}}, {}, {{"dem_cloth_robe", 2, {{7, 7}}}, {"dem_plate", 2, {{1, 1}}}}, noFlags);
    CHECK(trace.find("alloc 4096 n0;") == 0);
    run(0, 9, {{"plate", 4}}, {}, {{"def_plate", 2, {{3, 3}}}, {"bm_plate", 2, {{1, 1}}}}, noFlags);
    CHECK(trace.find("alloc 12288 n0;") == 0);
    // digit trimming stops at the character just below '0' and just above '9', and takes '0' and '9'
    run(0, 0, {{"plate/", 4}}, {}, {{"bm_plate/", 2, {{1, 1}}}, {"bm_plate", 2, {{5, 5}}}}, noFlags);
    CHECK(trace.find("alloc 4096 n0;") == 0);
    run(0, 0, {{"plate:", 4}}, {}, {{"bm_plate:", 2, {{1, 1}}}, {"bm_plate", 2, {{5, 5}}}}, noFlags);
    CHECK(trace.find("alloc 4096 n0;") == 0);
    run(0, 0, {{"plate09", 4}}, {}, {{"bm_plate", 2, {{1, 1}}}, {"bm_plate09", 2, {{5, 5}}}}, noFlags);
    CHECK(trace.find("alloc 4096 n0;") == 0);
    std::puts("player allocate armor cache passed");
    return 0;
}
