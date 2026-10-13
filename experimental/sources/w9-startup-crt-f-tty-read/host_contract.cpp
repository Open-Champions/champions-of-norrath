// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -o /tmp/c host_contract.cpp tty_read.cpp \
//            ../w9-startup-crt-f-tty-state/tty_state.cpp
#include <cstring>
#include <unistd.h>
#include "../w9-startup-crt-f-tty-state/tty_state.hpp"

static unsigned char queue_data[64];
static TtyQueue queue;
static int done_calls, count_when_peeked_bad;
extern "C" {
void QueuePeekReadDone(TtyQueue* q) {
    if (q != &queue) ++count_when_peeked_bad;
    ++done_calls;
    --q->count;
    ++q->readPtr;
}
int sceTtyRead(char* buffer, int count);
}
static void load(const char* text) {
    std::memset(queue_data, 0, sizeof queue_data);
    std::strcpy(reinterpret_cast<char*>(queue_data), text);
    queue.size = 256; queue.count = static_cast<int>(std::strlen(text)); queue.readPtr = queue_data; queue.writePtr = nullptr;
    tinfo.queue = &queue; done_calls = 0;
}
int main() {
    alarm(5);   // a wrong stop condition would wait forever on an empty queue; the alarm turns that into a failure
    char out[32];
    // stops after a line feed; the count includes it; the rest stays queued
    load("ab\ncd"); std::memset(out, 'x', sizeof out);
    if (sceTtyRead(out, 16) != 3 || std::memcmp(out, "ab\n", 3) != 0 || out[3] != 'x') return 1;
    if (done_calls != 3 || queue.count != 2 || *queue.readPtr != 'c') return 2;
    // carriage return ends a line as well
    load("q\rz"); std::memset(out, 'x', sizeof out);
    if (sceTtyRead(out, 16) != 2 || out[0] != 'q' || out[1] != '\r' || out[2] != 'x') return 3;
    // the requested count wins over the end of the data
    load("abcdef"); std::memset(out, 'x', sizeof out);
    if (sceTtyRead(out, 4) != 4 || std::memcmp(out, "abcd", 4) != 0 || out[4] != 'x' || done_calls != 4) return 4;
    // a count of zero or less reads nothing and never touches the queue
    load("abc"); std::memset(out, 'x', sizeof out);
    if (sceTtyRead(out, 0) != 0 || sceTtyRead(out, -5) != 0 || done_calls != 0 || out[0] != 'x') return 5;
    // a line feed in the last requested position is returned like any other
    load("ab\n"); 
    if (sceTtyRead(out, 3) != 3 || done_calls != 3) return 6;
    // the queue used is the one in tinfo at each step
    if (count_when_peeked_bad != 0) return 7;
    // a single byte request
    load("\n"); if (sceTtyRead(out, 1) != 1 || out[0] != '\n') return 8;
    return 0;
}
