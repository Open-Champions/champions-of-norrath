// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp mce_sif_rpc_end_func.cpp
extern "C" {
int semaid_timerfunc;
static int calls, seen = -1, signals_other;
int iSignalSema(int semaphore) { ++calls; seen = semaphore; return 4000 + semaphore; }
int mceSifRpcEndFunc(void*);
}
int main() {
    int dummy = 0;
    semaid_timerfunc = 17;
    if (mceSifRpcEndFunc(&dummy) != 4017 || calls != 1 || seen != 17) return 1;   // the live word is read; the result is passed on
    semaid_timerfunc = -3;
    if (mceSifRpcEndFunc(nullptr) != 3997 || calls != 2 || seen != -3) return 2;
    (void)signals_other;
    return 0;
}
