// Host check for runScreaming__Fv. Not an EE plan input.
#include <cstring>
#include <string>
#include "run_screaming.cpp"

extern "C" {
unsigned char stream[1356];
unsigned char gCd[64];
unsigned char sbuff[64];
unsigned char sbuffin[64];
}
static std::string trace;
static int busy = 0, queue_result = 0, layer_ok = 1;
static unsigned char rpc_out[64];
static int load_calls = 0;
static std::string load_text[4];
static char* load_done_seen = nullptr;

extern "C" int cdIsBusy__Fv() { trace += "B;"; return busy; }
extern "C" int runStreamQueing__Fv() { trace += "Q;"; return queue_result; }
extern "C" int cdCheckForFileInCurrentLayer__FPc(char* n) { trace += std::string("C[") + n + "];"; return layer_ok; }
extern "C" void cdLoadAsyncIOP__FPciT0ii(char* n, int dest, char* done, int size, int offset) {
    trace += std::string("L[") + n + "," + std::to_string(dest) + "," + std::to_string(size) + "," + std::to_string(offset) + "];";
    load_done_seen = done;
    ++load_calls;
}
extern "C" int sceSifCallRpc(void* client, int op, int mode, void* send, int ssize, void* recv, int rsize, void* f, void* a) {
    trace += "R" + std::to_string(op) + ":" + std::to_string(ssize) + ":" + std::to_string(rsize) + (client == gCd ? "" : "?")
        + (send == sbuffin ? "" : "?") + (recv == sbuff ? "" : "?") + (mode || f || a ? "?" : "") + ";";
    if (op == 80) std::memcpy(sbuff, rpc_out, 64);
    return 0;
}
static void w32(unsigned char* p, int v) { for (int i = 0; i < 4; ++i) p[i] = (unsigned char)((unsigned)v >> (8 * i)); }
static unsigned char* rec(int i) { return stream + i * 452; }
static void reset() {
    trace.clear(); busy = 0; queue_result = 0; layer_ok = 1;
    std::memset(stream, 0, sizeof stream); std::memset(rpc_out, 0, sizeof rpc_out);
    std::memset(sbuffin, 0xee, sizeof sbuffin);
    loadDone = 1; loadInProgress = 0; loadVoice = loadAmount = loadSampleRate = loadStereo = preloadAmount = 0;
    load_calls = 0;
}

int main() {
    // Busy or not done: nothing but the first reads.
    reset(); busy = 1; runScreaming__Fv();
    if (trace != "B;") return 1;
    reset(); loadDone = 0; runScreaming__Fv();
    if (trace != "B;") return 2;

    // Idle, queue stops: RPC 80 only, no loads.
    reset(); queue_result = 1; runScreaming__Fv();
    if (trace != "B;R80:0:64;Q;") return 3;

    // Finished load on voice 1: updates, message, RPC 64, then the common path.
    reset();
    loadInProgress = 1; loadVoice = 1; loadAmount = 4096; loadSampleRate = 22050; loadStereo = 1; preloadAmount = 0;
    std::strcpy((char*)rec(1), "snd");
    w32(rec(1) + 16, 1000); w32(rec(1) + 20, 10000); w32(rec(1) + 24, 1000);   // end equals the old position
    queue_result = 1;
    runScreaming__Fv();
    if (trace != "B;R64:16:64;R80:0:64;Q;") return 4;
    if (word_at(rec(1) + 16) != 5096 || word_at(rec(1) + 20) != 5904) return 5;
    {
        const int expect[4] = {1, 4096, 22050, 1 | (1 << 16)};
        for (int i = 0; i < 4; ++i) if (word_at(sbuffin + i * 4) != expect[i]) return 6;
    }
    if (loadInProgress != 0) return 7;
    // End word different: atEnd zero; zero amount sends nothing.
    reset(); loadInProgress = 1; loadVoice = 0; loadAmount = 0; std::strcpy((char*)rec(0), "snd");
    w32(rec(0) + 16, 8); w32(rec(0) + 20, 8); w32(rec(0) + 24, 9); queue_result = 1;
    runScreaming__Fv();
    if (trace != "B;R80:0:64;Q;") return 8;
    // Loop restart: remaining zero and flag bit 0.
    reset(); loadInProgress = 1; loadVoice = 2; loadAmount = 100; std::strcpy((char*)rec(2), "snd");
    w32(rec(2) + 16, 0); w32(rec(2) + 20, 100); w32(rec(2) + 24, 5000); w32(rec(2) + 28, 90000); w32(rec(2) + 32, 1);
    w32(rec(2) + 60, 5000); queue_result = 1;
    runScreaming__Fv();
    if (word_at(rec(2) + 20) != 90000 - ((5000 << 1) & -2048) || word_at(rec(2) + 16) != 5000 + ((5000 << 1) & -2048)) return 9;
    // No restart without the flag.
    reset(); loadInProgress = 1; loadVoice = 2; loadAmount = 100; std::strcpy((char*)rec(2), "snd");
    w32(rec(2) + 20, 100); w32(rec(2) + 32, 0); queue_result = 1;
    runScreaming__Fv();
    if (word_at(rec(2) + 20) != 0 || word_at(rec(2) + 16) != 100) return 10;
    // A record without a name skips the message and the restart.
    reset(); loadInProgress = 1; loadVoice = 2; loadAmount = 100;
    w32(rec(2) + 20, 100); w32(rec(2) + 32, 1); queue_result = 1;
    runScreaming__Fv();
    if (trace != "B;R80:0:64;Q;" || word_at(rec(2) + 16) != 100 || word_at(rec(2) + 20) != 0) return 11;
    // Preload: moved into the offset word and cleared when the first name is set.
    reset(); loadInProgress = 1; loadVoice = 0; loadAmount = 0; preloadAmount = 2048; std::strcpy((char*)rec(0), "a");
    w32(rec(0) + 36, 5); queue_result = 1;
    runScreaming__Fv();
    if (word_at(rec(0) + 36) != 5 + 2048 || preloadAmount != 0) return 12;
    // Preload with no first name but a second file: also moved.
    reset(); loadInProgress = 1; loadVoice = 0; loadAmount = 0; preloadAmount = 2048;
    w32(rec(0) + 448, 1); rec(0)[64] = 1; w32(rec(0) + 36, 5); queue_result = 1;
    runScreaming__Fv();
    if (word_at(rec(0) + 36) != 5 + 2048 || preloadAmount != 0) return 13;
    // Preload with neither: untouched.
    reset(); loadInProgress = 1; loadVoice = 0; loadAmount = 0; preloadAmount = 2048;
    w32(rec(0) + 36, 5); queue_result = 1;
    runScreaming__Fv();
    if (word_at(rec(0) + 36) != 5 || preloadAmount != 2048) return 14;

    // Voice loop: record 1 needs data. chunk 16384, received position 3100, played 1000, offset 0.
    reset();
    w32(rpc_out + 16, 3100); w32(rpc_out + 20, 1000);
    std::strcpy((char*)rec(1), "snd"); w32(rec(1) + 20, 50000); w32(rec(1) + 44, 0x100000); w32(rec(1) + 48, 16384);
    w32(rec(1) + 16, 777); w32(rec(1) + 52, 22050); w32(rec(1) + 56, 2);
    runScreaming__Fv();
    {
        // position = 3100 % 16384 = 3100; available = 16384 - ((3100-1000+16384) % 16384) - 2048 = 12336; to end = 13384 -> 12336 & -2048 = 10240
        const int avail = (16384 - ((3100 - 1000 + 16384) % 16384) - 2048);
        const int amount = (avail < 16384 - 3100 ? avail : 16384 - 3100) & -2048;
        std::string want = "B;R80:0:64;Q;C[snd];L[snd," + std::to_string(0x100000 + 3100) + "," + std::to_string((amount + 2047) & -2048) + ",777];";
        if (trace != want) { std::fprintf(stderr, "got %s\nwant %s\n", trace.c_str(), want.c_str()); return 15; }
        if (loadInProgress != 1 || loadVoice != 1 || loadAmount != amount || loadSampleRate != 22050 || loadStereo != 2 || preloadAmount != 0) return 16;
        if ((signed char*)load_done_seen != &loadDone) return 17;
    }
    // Remaining smaller than the amount limits the load.
    reset(); w32(rpc_out + 0, 3000); w32(rpc_out + 4, 1000);
    std::strcpy((char*)rec(0), "snd"); w32(rec(0) + 20, 5000); w32(rec(0) + 44, 1); w32(rec(0) + 48, 16384);
    runScreaming__Fv();
    if (loadAmount != 5000 || trace.find(",6144,") == std::string::npos) return 18;
    // A file missing from the layer falls through to the second file.
    reset(); layer_ok = 0; w32(rpc_out + 0, 3000); w32(rpc_out + 4, 1000);
    std::strcpy((char*)rec(0), "snd"); w32(rec(0) + 20, 5000); w32(rec(0) + 44, 1); w32(rec(0) + 48, 16384);
    runScreaming__Fv();
    if (load_calls != 0 || trace.find("C[snd]") == std::string::npos) return 19;
    // Second file load.
    reset(); w32(rpc_out + 0, 3000); w32(rpc_out + 4, 1000);
    w32(rec(0) + 44, 1); w32(rec(0) + 48, 16384); w32(rec(0) + 448, 2); std::strcpy((char*)rec(0) + 64, "two");
    w32(rec(0) + 84, 4000); w32(rec(0) + 80, 300); w32(rec(0) + 52, 11025); w32(rec(0) + 56, 1);
    runScreaming__Fv();
    if (trace != "B;R80:0:64;Q;C[two];L[two,3001,4096,300];") { std::fprintf(stderr, "got %s\n", trace.c_str()); return 20; }
    if (word_at(rec(0) + 80) != 300 + 4000 || word_at(rec(0) + 84) != 0) return 21;
    if (preloadAmount != 4000 || loadAmount != 0 || loadVoice != 0 || loadInProgress != 1) return 22;
    // Flag bit 0 stops the second file.
    reset(); w32(rpc_out + 0, 3000); w32(rpc_out + 4, 1000);
    w32(rec(0) + 44, 1); w32(rec(0) + 48, 16384); w32(rec(0) + 448, 2); std::strcpy((char*)rec(0) + 64, "two");
    w32(rec(0) + 84, 4000); w32(rec(0) + 32, 1);
    runScreaming__Fv();
    if (load_calls != 0) return 23;
    // A record with no destination is skipped; the next voice is tried.
    reset(); w32(rpc_out + 16, 3000); w32(rpc_out + 20, 1000);
    std::strcpy((char*)rec(0), "zero"); w32(rec(0) + 20, 5000); w32(rec(0) + 48, 16384);
    std::strcpy((char*)rec(1), "one"); w32(rec(1) + 20, 5000); w32(rec(1) + 44, 1); w32(rec(1) + 48, 16384);
    runScreaming__Fv();
    if (loadVoice != 1 || load_calls != 1) return 24;
    // Too close to the next chunk (less than half a chunk available): skipped.
    reset(); w32(rpc_out + 0, 9100); w32(rpc_out + 4, 100);
    std::strcpy((char*)rec(0), "snd"); w32(rec(0) + 20, 5000); w32(rec(0) + 44, 1); w32(rec(0) + 48, 16384);
    runScreaming__Fv();
    if (load_calls != 0) return 25;
    return 0;
}
