// Observed texProcessLoad__Fi at 0x00170538, 2632 bytes.
// A per-frame state machine that streams texture pages for the current
// world. The argument is -1 ("choose from the surroundings") or one block id.
// A null world pointer (0x004844a0) returns at once. The state byte is
// loadInProgress (0x00484110): 0 idle, 1 waiting on the stream, 2 releasing
// evicted pages, 3 a one-frame delay. Any other value does nothing. Offsets
// below are into the world header (word fields): +88 first block id, +92 last
// block id, +96 segment count, +100 table of 8-byte block records (first word
// stream start, second word byte size), +104 float cell size, +124 staging
// base. A copy of worldViewCenter is taken at entry.
//
// State 3: the state becomes 2.
// State 2: for each of loadKillListSize slots in loadKillList: the count word
//  at the page address in texturePageLoc[slot] gives that many 64-byte
//  textures starting 64 bytes in, each passed to textureFreeTexture; then every
//  segment number in texturePageSegLoc[slot*128...] up to the 0xff terminator
//  has its segmentUsed byte cleared; texturePageBlock[slot] becomes -1 and
//  texturePageLoc[slot] becomes 0. Afterwards loadCompleteFlag is set,
//  the state becomes 1 and loadKillListSize and loadSegmentsLoaded become 0.
// State 1: nothing happens while loadCompleteFlag (0x00486981) is zero or the
//  stream is busy (cdIsBusyStreamingNonwithstanding). With
//  rest = round_up(size of loadPage's block, 2048) - loadSegmentsLoaded*0x20000:
//  * rest > 0: the first free segment (segmentUsed byte zero) is found; if
//    reallyLoad is set, cdStreamLoadAsync is started for min(rest, 0x20000)
//    bytes into staging + segment*0x20000 with loadCompleteFlag as the done
//    byte; either way segment is recorded in texturePageSegLoc[loadSlot*128 +
//    loadSegmentsLoaded], marked used, and loadSegmentsLoaded is incremented.
//  * otherwise: cdEndStream is called if reallyLoad; the page address
//    staging + firstSegment*0x20000 is stored in texturePageLoc[loadSlot];
//    remaining segment entries from loadSegmentsLoaded to 127 are set to 0xff;
//    the state and loadCompleteFlag are cleared. If reallyLoad: the page's
//    count word is read (dumpMem of 1024 bytes when it exceeds 16000) and
//    textureInitTexture is called for each 64-byte texture; if not, the count
//    word is set to zero. Finally texturePageBlock[loadSlot] = loadPage and
//    texturePageTable[loadPage] = loadSlot.
// State 0: returns while cdIsBusy. Otherwise cell = floorf(position /
//  world cell size) + 50 for x and y. For -1 the nine block ids of the 3x3
//  neighbourhood are looked up in a 100-wide map at texmap (halfword per cell,
//  indices texmapstart..texmapend) and must lie within the world's block id
//  range, else -1; otherwise slot 0 is the argument and the rest are zero.
//  Resident slots matching any of the nine are flagged. Among nine ids that
//  are not resident, in range and with a nonzero size, the one with the
//  smallest squared distance wins; the distance uses the x cell index for both
//  axes (observed in the original). With no winner texturesAreLoading is cleared and
//  nothing else changes. Otherwise segments are made available by walking the
//  unflagged resident slots in order, adding their blocks to loadKillList,
//  clearing texturePageTable for them and counting the segments they free until
//  enough (ceil(size / 0x20000)) are free; if no slot is left, loadSlot gets
//  the previous candidate and the function returns. loadSlot is set to the
//  chosen slot; a pending decode (texDecodeStarted) is cancelled by clearing
//  matching uncompMap words, terminating and restarting the decompress thread.
//  Then the queue head and tail are cleared, the file name "%s.tex" with
//  engineCurrentLevelName is built, the state becomes 3, loadPage becomes the
//  block, reallyLoad is set to whether its stream start word is nonzero and,
//  if so, cdStartStream is called, and texturesAreLoading is set to 1.
// Observed behaviours that look unintended are reproduced as found.
// Record meanings beyond these offsets are not established.
#if defined(NORRATH_HOST_TEXLOAD)
extern unsigned char* norrath_tex_arena;
#endif

extern "C" {
extern void* world;
extern float worldViewCenter[3];
extern signed char loadInProgress;
extern int loadKillListSize;
extern int loadSegmentsLoaded;
extern signed char reallyLoad __asm__("reallyLoad.873");
extern unsigned char texturesAreLoading;
extern unsigned texDecodeStarted;
extern int decompressThreadid;
extern int decodeQTail;
extern int decodeQHead;
extern signed char loadSlot;
extern signed char loadCompleteFlag;
extern short loadPage;
extern short texmapstart;
extern short texmapend;
extern unsigned char texmap[];
extern int texturePageBlock[];
extern unsigned char texturePageSegLoc[];
extern signed char texturePageTable[];
extern unsigned texturePageLoc[];
extern signed char loadKillList[];
extern unsigned char segmentUsed[];
extern unsigned char uncompMap[];
extern char engineCurrentLevelName[];

int cdIsBusy__Fv();
int cdIsBusyStreamingNonwithstanding__Fv();
void cdStartStream__FPci(char* name, int start);
void cdEndStream__Fv();
void cdStreamLoadAsync__FPcT0i(char* destination, char* done, int size);
void dumpMem__FPvi(void* start, int size);
void textureFreeTexture__FP8_texture(void* texture);
void textureInitTexture__FP12_worldHeaderP8_texturePUcPc(void* world, void* texture, unsigned char* segments, char* staging);
int TerminateThread(int thread);
int StartThread(int thread, void* argument);
int sprintf(char* buffer, const char* format, ...);
void* memset(void* destination, int value, __SIZE_TYPE__ count);
float floorf(float value);
}

namespace {
inline unsigned word(const unsigned char* base, unsigned offset) {
    return *reinterpret_cast<const unsigned*>(base + offset);
}
inline unsigned char* address(unsigned bits) {
#if defined(NORRATH_HOST_TEXLOAD)
    return norrath_tex_arena + bits;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(bits));
#endif
}
inline unsigned char* block_record(const unsigned char* header, int block) {
    return address(word(header, 100)) + (block - static_cast<int>(word(header, 88))) * 8;
}
}

extern "C" void texProcessLoad__Fi(int requested) {
    unsigned char* header = static_cast<unsigned char*>(world);
    if (header == nullptr) return;

    float position[3];
    position[0] = worldViewCenter[0];
    position[1] = worldViewCenter[1];
    position[2] = worldViewCenter[2];
    const int state = loadInProgress;

    if (state == 1) {
        if (loadCompleteFlag == 0) return;
        if (cdIsBusyStreamingNonwithstanding__Fv() != 0) return;
        const int size = static_cast<int>(word(block_record(header, loadPage), 4));
        const int rounded = (size + 2047) & -2048;
        const int rest = rounded - (loadSegmentsLoaded << 17);
        if (rest > 0) {
            int segment = 0;
            while (segmentUsed[segment] != 0) ++segment;
            const int chunk = rest > 0x20000 ? 0x20000 : rest;
            if (reallyLoad != 0) {
                cdStreamLoadAsync__FPcT0i(
                    reinterpret_cast<char*>(address(word(header, 124)) + (segment << 17)),
                    reinterpret_cast<char*>(&loadCompleteFlag), chunk);
            }
            texturePageSegLoc[loadSlot * 128 + loadSegmentsLoaded] = static_cast<unsigned char>(segment);
            segmentUsed[segment] = 1;
            loadSegmentsLoaded = loadSegmentsLoaded + 1;
            return;
        }
        const int loaded = loadSegmentsLoaded;
        const int really = reallyLoad;
        if (really != 0) {
            cdEndStream__Fv();
        }
        const int slot = loadSlot;
        unsigned char* row = texturePageSegLoc + slot * 128;
        texturePageLoc[slot] = word(header, 124) + (static_cast<unsigned>(row[0]) << 17);
        for (int i = loaded; i < 128; ++i) row[i] = 0xff;
        loadInProgress = 0;
        loadCompleteFlag = 0;
        if (really == 0) {
            *reinterpret_cast<int*>(address(texturePageLoc[slot])) = 0;
        } else {
            unsigned char* page = address(texturePageLoc[slot]);
            const int count = *reinterpret_cast<int*>(page);
            if (count > 16000) {
                dumpMem__FPvi(page, 1024);
            }
            for (int k = 0; k < count; ++k) {
                textureInitTexture__FP12_worldHeaderP8_texturePUcPc(
                    world, page + 64 + k * 64, texturePageSegLoc + loadSlot * 128,
                    reinterpret_cast<char*>(address(word(header, 124))));
            }
        }
        const int final_slot = loadSlot;
        texturePageBlock[final_slot] = loadPage;
        texturePageTable[loadPage] = static_cast<signed char>(final_slot);
        return;
    }

    if (state == 0) {
        if (cdIsBusy__Fv() != 0) return;
        const float cell_size = *reinterpret_cast<const float*>(header + 104);
        const int cell_x = static_cast<int>(floorf(position[0] / cell_size)) + 50;
        const int cell_y = static_cast<int>(floorf(position[1] / cell_size)) + 50;
        const int first_block = static_cast<int>(word(header, 88));
        const int last_block = static_cast<int>(word(header, 92));
        int cells[9];
        if (requested == -1) {
            for (int row = 0; row < 3; ++row) {
                for (int column = 0; column < 3; ++column) {
                    const int cell = (cell_y + row) * 100 + cell_x - 101 + column;
                    int value = -1;
                    if (!(cell < texmapstart) && !(texmapend < cell)) {
                        const int mapped = *reinterpret_cast<const short*>(texmap + (cell - texmapstart) * 2);
                        value = mapped;
                        if (mapped < first_block || last_block < mapped) value = -1;
                    }
                    cells[row * 3 + column] = value;
                }
            }
        } else {
            memset(cells, 0, 36);
            cells[0] = requested;
        }
        char flags[9];
        memset(flags, 0, 9);
        for (int s = 0; s < 9; ++s) {
            const int block = texturePageBlock[s];
            if (block == -1) continue;
            bool found = block == cells[0];
            for (int k = 1; !found && k < 9; ++k) found = block == cells[k];
            if (found) flags[s] = 1;
        }

        float best = 10000000000.0f;
        int chosen = -1;
        for (int k = 0; k < 9; ++k) {
            const int block = cells[k];
            bool resident = block == texturePageBlock[0];
            for (int j = 1; !resident && j < 9; ++j) resident = block == texturePageBlock[j];
            if (resident) continue;
            if (block < first_block) continue;
            if (last_block < block) continue;
            if (word(block_record(header, block), 4) == 0) continue;
            const float fx = static_cast<float>(cell_x + k % 3 - 51) + 0.5f;
            const float fy = static_cast<float>(cell_x + k / 3 - 51) + 0.5f;
            const float dx = fx * cell_size - position[0];
            const float dy = fy * cell_size - position[1];
            const float distance = dx * dx + dy * dy;
            if (distance < best) {
                best = distance;
                chosen = block;
            }
        }
        if (chosen == -1) {
            texturesAreLoading = 0;
            return;
        }

        const int size = static_cast<int>(word(block_record(header, chosen), 4));
        loadKillListSize = 0;
        int free_segments = 0;
        const int segment_count = static_cast<int>(word(header, 96));
        for (int i = 0; i < segment_count; ++i) {
            if (segmentUsed[i] == 0) ++free_segments;
        }
        const int needed = (size + 0x1ffff) >> 17;
        bool short_of = free_segments < needed;
        int previous = -1;
        int slot;
        for (;;) {
            slot = 0;
            if (flags[0] != 0) {
                slot = 1;
                while (slot < 9 && flags[slot] != 0) ++slot;
            }
            if (slot == 9) {
                loadSlot = static_cast<signed char>(previous);
                return;
            }
            flags[slot] = 1;
            const int block = texturePageBlock[slot];
            if (block != -1 && texturePageTable[block] != -1) {
                const int index = loadKillListSize;
                texturePageTable[block] = -1;
                loadSlot = static_cast<signed char>(previous);
                loadKillList[index] = static_cast<signed char>(slot);
                loadKillListSize = index + 1;
                free_segments += static_cast<int>(word(block_record(header, block), 4) + 0x1ffffu) >> 17;
                short_of = free_segments < needed;
            }
            previous = slot;
            if (short_of) continue;
            if (static_cast<signed char>(slot) == -1) continue;
            break;
        }
        loadSlot = static_cast<signed char>(slot);
        if (texDecodeStarted != 0) {
            for (int i = 0; i < 1280; ++i) {
                unsigned* entry = reinterpret_cast<unsigned*>(uncompMap + i * 8);
                if (*entry == texDecodeStarted) *entry = 0;
            }
            texDecodeStarted = 0;
            TerminateThread(decompressThreadid);
            StartThread(decompressThreadid, nullptr);
        }
        decodeQTail = 0;
        decodeQHead = decodeQTail;
        char name[80];
        loadInProgress = 3;
        loadPage = static_cast<short>(chosen);
        sprintf(name, "%s.tex", engineCurrentLevelName);
        const unsigned start = word(block_record(header, chosen), 0);
        if (start != 0) {
            reallyLoad = 1;
            cdStartStream__FPci(name, static_cast<int>(start));
        } else {
            reallyLoad = 0;
        }
        texturesAreLoading = 1;
        return;
    }

    if (state == 2) {
        const int count = loadKillListSize;
        for (int i = 0; i < count; ++i) {
            const int slot = loadKillList[i];
            unsigned char* page = address(texturePageLoc[slot]);
            const int textures = *reinterpret_cast<int*>(page);
            for (int k = 0; k < textures; ++k) {
                textureFreeTexture__FP8_texture(page + 64 + k * 64);
            }
            const unsigned char* row = texturePageSegLoc + slot * 128;
            if (row[0] != 0xff) {
                int j = 0;
                do {
                    segmentUsed[row[j]] = 0;
                    ++j;
                } while (row[j] != 0xff);
            }
            texturePageBlock[slot] = -1;
            texturePageLoc[slot] = 0;
        }
        loadCompleteFlag = 1;
        loadInProgress = static_cast<signed char>(state - 1);
        loadKillListSize = 0;
        loadSegmentsLoaded = 0;
        return;
    }

    if (state == 3) {
        loadInProgress = static_cast<signed char>(state - 1);
    }
}
