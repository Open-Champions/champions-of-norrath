#include "alarm_rdata.cpp"

static int send_result;
static int sends;
static unsigned sent_command, sent_words[3];
static void* sent_packet;
static int sent_size;
extern "C" int isceSifSendCmd(unsigned command, void* packet, int size, unsigned a, unsigned b, unsigned c) {
    ++sends;
    sent_command = command; sent_packet = packet; sent_size = size;
    sent_words[0] = a; sent_words[1] = b; sent_words[2] = c;
    return send_result;
}

int main() {
    RpcDataReply reply = {};
    reply.word_9 = 0x11;
    reply.word_10 = 0x22;
    reply.word_11 = 0x33;
    send_result = 0;
    if (_alarm_rdata(-1, 0x1234, 0x4321, &reply) != 0x800u) return 1;
    if (sends != 1 || sent_command != 0x80000008u || sent_packet != &reply || sent_size != 64) return 2;
    if (sent_words[0] != 0x11 || sent_words[1] != 0x22 || sent_words[2] != 0x33) return 3;
    send_result = 1;
    if (_alarm_rdata(5, 0, 0, &reply) != 0) return 4;
    send_result = -1;
    if (_alarm_rdata(5, 0, 0, &reply) != 0) return 5;
    if (sends != 3) return 6;
    return 0;
}
