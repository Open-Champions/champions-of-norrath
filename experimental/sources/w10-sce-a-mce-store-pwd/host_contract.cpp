// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -o /tmp/c host_contract.cpp mce_store_pwd.cpp
#include <cstring>
extern "C" {
int semaid_timerfunc = 21;
unsigned char currentDir[2048];
static int signalled = -1, signals;
int iSignalSema(int semaphore) { signalled = semaphore; ++signals; return 900 + semaphore; }
int mceStorePwd(void* destination);
}
int main() {
    static char out[2048];
    std::memset(out, 0x7e, sizeof out);
    // null destination: nothing copied, semaphore still signalled and its result returned
    std::strcpy(reinterpret_cast<char*>(currentDir), "mc0:/BASLUS-20565");
    if (mceStorePwd(nullptr) != 921 || signalled != 21 || signals != 1) return 1;
    if (out[0] != 0x7e) return 2;
    // copies the text and the terminator, nothing more
    if (mceStorePwd(out) != 921 || std::strcmp(out, "mc0:/BASLUS-20565") != 0 || out[18] != 0x7e) return 3;
    // empty text still terminates the destination
    currentDir[0] = 0; out[0] = 0x7e;
    mceStorePwd(out);
    if (out[0] != 0 || out[1] != 0x7e) return 4;
    // a long text is cut at 1023 characters plus the terminator
    std::memset(currentDir, 'k', 1500); currentDir[1500] = 0;
    std::memset(out, 0x7e, sizeof out);
    mceStorePwd(out);
    if (out[1022] != 'k' || out[1023] != 0 || out[1024] != 0x7e) return 5;
    // 1023 characters fit unchanged; 1024 are cut to 1023
    std::memset(currentDir, 'q', 1023); currentDir[1023] = 0;
    std::memset(out, 0x7e, sizeof out);
    mceStorePwd(out);
    if (out[1022] != 'q' || out[1023] != 0 || out[1024] != 0x7e) return 6;
    std::memset(currentDir, 'r', 1024); currentDir[1024] = 0;
    std::memset(out, 0x7e, sizeof out);
    mceStorePwd(out);
    if (out[1022] != 'r' || out[1023] != 0 || out[1024] != 0x7e) return 7;
    // the semaphore word is read live
    semaid_timerfunc = -1;
    if (mceStorePwd(nullptr) != 899 || signalled != -1) return 8;
    return 0;
}
