// Host check only; not a build input.
#include <string>
#include <vector>
static std::vector<std::string> log_;
static int searchResults[4], searchIndex, errors[8], errorIndex, readyResults[8], readyIndex, viResult;
extern "C" int printf(const char* format, ...) {
    log_.push_back(format);
    return 0;
}
extern "C" int sceCdSearchFile(void*, const char*) { return searchResults[searchIndex++]; }
extern "C" int sceCdGetError() { return errors[errorIndex++]; }
extern "C" int sceCdDiskReady(int) { return readyResults[readyIndex++]; }
extern "C" int VICdSearchFile__FPCcPUiT1(const char*, unsigned* a, unsigned* b) {
    if (viResult >= 0) { *a = 0x111; *b = 0x222; }
    return viResult;
}
extern "C" void VIEEFileDelay__Fi(int ticks) { log_.push_back("delay" + std::to_string(ticks)); }
int norrath_file_iop_stream __asm__("_6VIFile$sIopStream");
#include "cd_search_file.cpp"
int main() {
    unsigned rec[2] = {1, 2};
    norrath_file_iop_stream = 1;
    viResult = -1;
    if (CDSearchFile__6VIFileP10sceCdlFILEPCc(0, rec, "x") != 0 || rec[0] != 1) return 1;
    viResult = 3;
    if (CDSearchFile__6VIFileP10sceCdlFILEPCc(0, rec, "x") != 1 || rec[0] != 0x111 || rec[1] != 0x222) return 2;
    norrath_file_iop_stream = 0;
    searchResults[0] = 7;
    if (CDSearchFile__6VIFileP10sceCdlFILEPCc(0, rec, "x") != 7 || searchIndex != 1 || errorIndex != 0) return 3;
    searchIndex = 0; searchResults[0] = 0; errors[0] = 0;
    if (CDSearchFile__6VIFileP10sceCdlFILEPCc(0, rec, "x") != 0 || errorIndex != 1 || !log_.empty()) return 4;
    // failure with an error: print, wait through two not-ready polls, search again and succeed
    searchIndex = 0; errorIndex = 0; readyIndex = 0;
    searchResults[0] = 0; searchResults[1] = 9;
    errors[0] = 5; errors[1] = 5;
    readyResults[0] = 6; readyResults[1] = 6; readyResults[2] = 2;
    log_.clear();
    if (CDSearchFile__6VIFileP10sceCdlFILEPCc(0, rec, "x") != 9) return 5;
    if (log_.size() != 5 || log_[0] != "CDSearchFile: Error %d\n") return 6;
    if (log_[1] != "CDSearchFile: Waiting for disk ready\n" || log_[2] != "delay480") return 7;
    if (readyIndex != 3 || searchIndex != 2) return 8;
    return 0;
}
