#include "cdsync.cpp"
#include <cstdarg>
#include <cstring>
#include <string>
#include <vector>

int norrath_file_iop_stream __asm__("_6VIFile$sIopStream") = 0;

static std::vector<std::string> calls;
static std::vector<int> sync_results, error_results, ready_results;
static size_t sync_at, error_at, ready_at;
static int read_args[4];

static int next(const std::vector<int>& list, size_t& at) { return at + 1 < list.size() ? list[at++] : list.back(); }
extern "C" int sceCdSync(int m) { calls.push_back("sync" + std::to_string(m)); return next(sync_results, sync_at); }
extern "C" int sceCdGetError() { calls.push_back("error"); return next(error_results, error_at); }
extern "C" int sceCdDiskReady(int m) { calls.push_back("ready" + std::to_string(m)); return next(ready_results, ready_at); }
extern "C" unsigned sceCdRead(unsigned a, unsigned b, unsigned c, unsigned d) {
    calls.push_back("read"); read_args[0] = a; read_args[1] = b; read_args[2] = c; read_args[3] = d; return 1;
}
extern "C" int VICdStreamSync__Fi(int m) { calls.push_back("ssync" + std::to_string(m)); return next(sync_results, sync_at); }
extern "C" int VICdStreamStatus__Fv() { calls.push_back("status"); return next(error_results, error_at); }
extern "C" int VICdStreamDiskReady__Fv() { calls.push_back("sready"); return next(ready_results, ready_at); }
extern "C" int VICdStreamRead__FUiUiPvi(unsigned a, unsigned b, unsigned c, int d) {
    calls.push_back("sread"); read_args[0] = a; read_args[1] = b; read_args[2] = c; read_args[3] = d; return 1;
}
extern "C" void VIEEFileDelay__Fi(int t) { calls.push_back("delay" + std::to_string(t)); }
extern "C" int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int value = va_arg(args, int);
    va_end(args);
    calls.push_back(std::string("printf:") + format + "|" + std::to_string(value));
    return 0;
}

int main() {
    for (int stream = 0; stream < 2; ++stream)
    for (int retry : {0, 1, 9})
    for (int first : {0, 3, -2})
    for (std::vector<int> errors : {std::vector<int>{0}, {5, 0}, {5, 6, 0}, {7, 8, 9, 0, 0}})
    for (std::vector<int> ready : {std::vector<int>{stream ? 0 : 2}, std::vector<int>{stream ? -1 : 0, stream ? -3 : 1, stream ? 0 : 2}}) {
        calls.clear(); sync_at = error_at = ready_at = 0;
        sync_results = {first, 11, 12, 0};
        error_results = errors; ready_results = ready;
        norrath_file_iop_stream = stream;
        alignas(8) unsigned char file[256];
        std::memset(file, 0x6b, sizeof file);
        int* words = reinterpret_cast<int*>(file + 112);
        words[0] = retry; words[1] = 100; words[2] = 200; words[3] = 300; words[4] = 400;
        int r = CDSync__6VIFilei(file, 4);
        std::string syncName = stream ? "ssync" : "sync", errName = stream ? "status" : "error", readyName = stream ? "sready" : "ready";
        if (calls[0] != syncName + "4") return 1;
        if (*reinterpret_cast<int*>(file + 112) != (first != 0 ? retry : 0)) return 2;
        if (first != 0) {
            if (r != first || calls.size() != 1) return 3;
            continue;
        }
        // Model the loop from the documented behavior.
        size_t errors_seen = 0;
        std::vector<std::string> want = {syncName + "4"};
        int result = 0, syncs = 1;   // sync_results[0] was consumed
        size_t e = 0, rd = 0, sy = 1;
        auto nextOf = [](const std::vector<int>& v, size_t& at) { return at + 1 < v.size() ? v[at++] : v.back(); };
        while (true) {
            want.push_back(errName);
            if (nextOf(errors, e) == 0 || retry == 0) break;
            want.push_back(errName);
            int printed = nextOf(errors, e);
            want.push_back(std::string("printf:CDSync: Error %d, restarting previous read\n|") + std::to_string(printed));
            for (;;) {
                if (stream) want.push_back("sready"); else want.push_back("ready0");
                int v = nextOf(ready, rd);
                if (stream ? v >= 0 : v == 2) break;
                want.push_back(std::string("printf:CDSync: Waiting for ") + (stream ? "disc" : "disk") + " ready\n|" + "0");
                want.push_back("delay480");
            }
            want.push_back(stream ? "ssync0" : "sync0");
            want.push_back(stream ? "sread" : "read");
            want.push_back(stream ? "ssync0" : "sync0");
            nextOf(sync_results, sy); result = nextOf(sync_results, sy);
            ++errors_seen;
            if (errors_seen > 8) return 90;
        }
        (void)syncs;
        if (r != result) return 8;
        if (calls.size() != want.size()) return 4;
        for (size_t i = 0; i < want.size(); ++i) {
            std::string got = calls[i];
            if (got.rfind("printf:", 0) == 0 && want[i].rfind("printf:CDSync: Waiting", 0) == 0) got = got.substr(0, got.rfind('|') + 1) + "0";
            if (got != want[i]) return 5;
        }
        if (errors_seen != 0) {
            if (read_args[0] != 100 || read_args[1] != 200 || read_args[2] != 300) return 6;
            if (read_args[3] != (stream ? 2 : 400)) return 7;
        }
    }
    return 0;
}
