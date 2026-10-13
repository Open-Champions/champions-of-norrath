// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp lgaud_a_sync.cpp
#include <unistd.h>
extern "C" {
int norrath_lgaud_semaphore_view = 5;
unsigned char norrath_lgaud_rpc_client_view[40];
unsigned norrath_lgaud_async_record_view[4];
static int stat_calls, busy_polls; static void* stat_client; static int status_value;
int sceSifCheckStatRpc(void* client) {
    ++stat_calls; stat_client = client;
    if (busy_polls > 0) { --busy_polls; return 1; }
    return status_value;
}
unsigned lgAudASync(int poll_only, unsigned* result);
}
int main() {
    alarm(10);   // a wrong wait loop would never end
    unsigned out = 0xaaaa;
    norrath_lgaud_async_record_view[2] = 0x600d;
    norrath_lgaud_semaphore_view = -1;
    if (lgAudASync(1, &out) != 0x8000000au || lgAudASync(0, &out) != 0x8000000au || stat_calls != 0 || out != 0xaaaa) return 1;
    norrath_lgaud_semaphore_view = 5;
    // poll: a busy status is returned as it is and nothing is stored
    status_value = 7;
    if (lgAudASync(1, &out) != 7 || out != 0xaaaa || stat_calls != 1 || stat_client != norrath_lgaud_rpc_client_view) return 2;
    status_value = -1;
    if (lgAudASync(1, &out) != 0xffffffffu || out != 0xaaaa) return 3;
    // poll: idle stores the record word and returns 0; a null pointer is allowed
    status_value = 0; stat_calls = 0;
    if (lgAudASync(1, &out) != 0 || out != 0x600d || stat_calls != 1) return 4;
    if (lgAudASync(1, nullptr) != 0) return 5;
    // wait: polls until idle, then the same store
    out = 0xaaaa; busy_polls = 3; stat_calls = 0; status_value = 0;
    if (lgAudASync(0, &out) != 0 || stat_calls != 4 || out != 0x600d) return 6;
    busy_polls = 2; stat_calls = 0;
    if (lgAudASync(0, nullptr) != 0 || stat_calls != 3) return 7;
    return 0;
}
