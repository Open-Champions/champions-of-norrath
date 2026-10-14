// Host contract for lumpLoadGlob__FPc. Not an EE plan input.
// Build: c++ -std=c++17 -DNORRATH_HOST_CONTRACT host_check.cpp
#define NORRATH_HOST_CONTRACT 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "lump_load_glob.cpp"

extern "C" {
LumpEntry lumps[128];
char globDir[2080];
int nmLumps;
int noNewVags;
char engineCurrentLevelName[80];
}
static std::vector<std::string> events;
static int null_stores;
static std::vector<unsigned char> directory;      // what the first cdLoad delivers
static std::vector<std::string> blocks_storage;
static char* alloc_results[16];
static int alloc_index;
static std::vector<int> init_results;
static int init_index;
static int ram_values[64], ram_index;
static int no_new_vags_seen[16], seen_count;
static void* alloc_label_seen[16];
static int alloc_size_seen[16], alloc_calls;
static int realloc_size_seen[16], realloc_calls;
static void* realloc_ptr_seen[16];
static int cd_len_seen[16], cd_off_seen[16], cd_calls;
static void* cd_dest_seen[16];
static char* cd_name_seen[16];

extern "C" void norrath_host_store_absolute(unsigned address, unsigned value) { if (address == 0 && value == 0) ++null_stores; }
extern "C" void cdLoad__FPcPvii(char* name, void* destination, int requested, int byte_offset) {
    events.push_back("cdLoad");
    if (cd_calls < 16) { cd_len_seen[cd_calls] = requested; cd_off_seen[cd_calls] = byte_offset; cd_dest_seen[cd_calls] = destination; cd_name_seen[cd_calls] = name; }
    ++cd_calls;
    if (destination == globDir) std::memcpy(globDir, directory.data(), directory.size());
}
extern "C" void FlushCache(int mode) { events.push_back(mode == 0 ? "flush0" : "flushN"); }
extern "C" void* memAlloc__FiPc(int bytes, char* label) {
    events.push_back("alloc");
    alloc_size_seen[alloc_calls] = bytes; alloc_label_seen[alloc_calls] = label; ++alloc_calls;
    return alloc_results[alloc_index++];
}
extern "C" int initLump__FPv(void*) {
    events.push_back("init");
    no_new_vags_seen[seen_count++] = noNewVags;
    return init_results[init_index++];
}
extern "C" void* memRealloc__FPvi(void* p, int size) {
    events.push_back("realloc");
    realloc_ptr_seen[realloc_calls] = p; realloc_size_seen[realloc_calls] = size; ++realloc_calls;
    return p;
}
extern "C" int soundGetFreeRam__Fv() { events.push_back("ram"); return ram_values[ram_index++]; }

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }

static void add_record(int index, const char* name, int size) {
    directory.resize(2048, 0);
    std::memset(&directory[40 * index], 0, 40);
    std::strcpy(reinterpret_cast<char*>(&directory[40 * index]), name);
    for (int i = 0; i < 4; ++i) directory[40 * index + 36 + i] = static_cast<unsigned char>(static_cast<unsigned>(size) >> (8 * i));
}
static char block_a[64], block_b[64], block_c[64];
static void reset() {
    events.clear(); null_stores = 0; directory.clear(); alloc_index = 0; init_index = 0; ram_index = 0; seen_count = 0;
    alloc_calls = realloc_calls = cd_calls = 0;
    std::memset(lumps, 0, sizeof lumps); std::memset(globDir, 0x7f, sizeof globDir);
    nmLumps = 0; noNewVags = 0;
    std::strcpy(engineCurrentLevelName, "somewhere");
    directory.assign(2048, 0);
}

int main() {
    // 1. Empty directory: read, flush, nothing else.
    reset();
    char file[] = "level.glob";
    lumpLoadGlob__FPc(file);
    expect(events.size() == 2 && events[0] == "cdLoad" && events[1] == "flush0", "empty directory: read the directory and flush only");
    expect(cd_len_seen[0] == 2048 && cd_off_seen[0] == 0 && cd_dest_seen[0] == globDir && cd_name_seen[0] == file, "directory read arguments");
    expect(nmLumps == 0, "nothing registered");

    // 2. One record: exact call order and table fields.
    reset();
    add_record(0, "plain.lmp", 5000);
    alloc_results[0] = block_a; init_results = {1000}; ram_values[0] = 900000; ram_values[1] = 850000;
    nmLumps = 3;
    std::strcpy(lumps[3].name, "stale");
    lumpLoadGlob__FPc(file);
    const char* want[] = {"cdLoad", "flush0", "alloc", "cdLoad", "ram", "init", "realloc", "ram"};
    bool order = events.size() == 8;
    for (int i = 0; order && i < 8; ++i) order = events[i] == want[i];
    expect(order, "call order: dir, flush, alloc, load, ram, init, realloc, ram");
    expect(alloc_size_seen[0] == 6144 + 256 && alloc_label_seen[0] == lumps[3].name, "allocation: padded size + 256, labelled with the slot");
    expect(cd_len_seen[1] == 6144 && cd_off_seen[1] == 2048 && cd_dest_seen[1] == block_a && cd_name_seen[1] == file, "lump read: padded length at offset 2048");
    expect(realloc_ptr_seen[0] == block_a && realloc_size_seen[0] == 5000 - 1000 + 256, "realloc to size - initLump result + 256");
    expect(std::strcmp(lumps[3].name, "plain.lmp") == 0 && lumps[3].lump == block_a, "name copied and lump pointer stored");
    expect(lumps[3].word24 == 4256 && lumps[3].word28 == 50000 && lumps[3].word32 == 0 && lumps[3].word20 == 0 && lumps[3].word36 == 1,
           "table words: new size, RAM used, zeros, loaded flag");
    expect(nmLumps == 4 && noNewVags == 0, "count incremented, noNewVags left at 0");
    expect(null_stores == 0, "no null store when the allocation succeeds");

    // 3. Several records, renames, offsets, sector rounding, vag flags, null allocation.
    reset();
    add_record(0, "gaydark1.lmp", 100);
    add_record(1, "vamplord.lmp", 2048);
    add_record(2, "gaydark2.lmp", 2049);
    add_record(3, "superorc.lmp", 1);
    alloc_results[0] = block_a; alloc_results[1] = block_b; alloc_results[2] = nullptr; alloc_results[3] = block_c;
    init_results = {10, 20, 30, 40};
    for (int i = 0; i < 8; ++i) ram_values[i] = 1000 * (i + 1);
    lumpLoadGlob__FPc(file);
    expect(nmLumps == 4, "four lumps registered");
    expect(std::strcmp(lumps[0].name, "faydark1.lmp") == 0 && std::strcmp(lumps[2].name, "faydark2.lmp") == 0, "gaydark names are replaced");
    expect(std::strcmp(lumps[1].name, "vamplord.lmp") == 0 && std::strcmp(lumps[3].name, "superorc.lmp") == 0, "others keep their names");
    expect(cd_off_seen[1] == 2048 && cd_off_seen[2] == 2048 + 2048 && cd_off_seen[3] == 2048 + 4096 && cd_off_seen[4] == 2048 + 4096 + 4096,
           "offsets grow by the padded sizes (2048, 2048, 4096)");
    expect(cd_len_seen[1] == 2048 && cd_len_seen[2] == 2048 && cd_len_seen[3] == 4096 && cd_len_seen[4] == 2048, "padded sizes");
    expect(alloc_label_seen[1] == lumps[1].name && alloc_label_seen[3] == lumps[3].name, "labels follow the table count");
    expect(no_new_vags_seen[0] == 0 && no_new_vags_seen[1] == 1 && no_new_vags_seen[2] == 0 && no_new_vags_seen[3] == 1,
           "noNewVags set during initLump for vamplord and superorc away from their levels");
    expect(noNewVags == 0, "noNewVags cleared at the end");
    expect(null_stores == 1, "one null allocation stores zero through address 0");
    expect(lumps[0].word28 == 1000 - 2000 + 0 || lumps[0].word28 == -1000, "RAM delta is first reading minus second");
    expect(lumps[1].lump == block_b && lumps[3].lump == block_c && lumps[2].lump == nullptr, "lump pointers per record");

    reset();
    noNewVags = 9;
    add_record(0, "plain.lmp", 10);
    alloc_results[0] = block_a; init_results = {0};
    lumpLoadGlob__FPc(file);
    expect(no_new_vags_seen[0] == 9 && noNewVags == 0, "a plain record leaves noNewVags alone during initLump, then clears it");

    // 4. The level name exempts the vag flag.
    reset();
    add_record(0, "vamplord.lmp", 10); add_record(1, "superorc.lmp", 10);
    alloc_results[0] = block_a; alloc_results[1] = block_b; init_results = {0, 0};
    std::strcpy(engineCurrentLevelName, "gothic2");
    lumpLoadGlob__FPc(file);
    expect(no_new_vags_seen[0] == 0 && no_new_vags_seen[1] == 1, "gothic2 exempts vamplord but not superorc");
    reset();
    add_record(0, "vamplord.lmp", 10); add_record(1, "superorc.lmp", 10);
    alloc_results[0] = block_a; alloc_results[1] = block_b; init_results = {0, 0};
    std::strcpy(engineCurrentLevelName, "ruinint1");
    lumpLoadGlob__FPc(file);
    expect(no_new_vags_seen[0] == 1 && no_new_vags_seen[1] == 0, "ruinint1 exempts superorc but not vamplord");
    if (failures) return 1;
    std::printf("lumpLoadGlob contract passed\n");
    return 0;
}
