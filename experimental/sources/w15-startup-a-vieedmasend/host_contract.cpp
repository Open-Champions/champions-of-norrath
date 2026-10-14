static unsigned fake_stat;
#define NORRATH_DMAC_STAT fake_stat
#include "dma_send.cpp"

#include <cstdio>

static int get_calls, send_calls, get_arg, send_ret, stat_at_send;
static sceDmaChan* send_chan;
static void* send_data;
static char chans[4];

sceDmaChan* sceDmaGetChan(int channel) {
    ++get_calls;
    get_arg = channel;
    return reinterpret_cast<sceDmaChan*>(&chans[channel & 3]);
}
int sceDmaSend(sceDmaChan* channel, void* tag) {
    ++send_calls;
    send_chan = channel;
    send_data = tag;
    stat_at_send = static_cast<int>(fake_stat);
    return send_ret;
}

int main() {
    char tag[4];
    send_ret = 11;
    for (int channel = -1; channel < 11; ++channel) {
        fake_stat = 0;
        get_calls = send_calls = 0;
        if (VIEEDMASend__FiPv(channel, tag) != 11) return 1;
        if (get_calls != 1 || send_calls != 1 || get_arg != channel) return 2;
        if (send_chan != reinterpret_cast<sceDmaChan*>(&chans[channel & 3]) || send_data != tag) return 3;
        const unsigned expected = channel == 1 ? 2u : 0u;
        if (fake_stat != expected || static_cast<unsigned>(stat_at_send) != expected) return 4;
    }
    send_ret = -5;
    if (VIEEDMASend__FiPv(1, tag) != -5) return 5;
    std::puts("VIEEDMASend contract passed");
    return 0;
}
