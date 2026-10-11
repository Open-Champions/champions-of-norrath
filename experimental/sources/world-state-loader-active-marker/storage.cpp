#include "storage.hpp"
// Observed lumpLoadCompletionFlag at 0x00484011: one file-backed zero byte in .sdata.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char norrath_loader_active_marker_view[1] NORRATH_LOADED(".sdata") = {
    0x00
};
}
