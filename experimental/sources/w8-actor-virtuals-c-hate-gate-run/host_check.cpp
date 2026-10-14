#define NORRATH_W8C_HOST 1
#include "run.cpp"
#include "../w8-actor-virtuals-c-hate-run-common/host_hate_check.hpp"
unsigned char norrath_anttrap_msg_run_HateGateParticle_bytes[80];
int main() {
    hate_check(msg_run__8HateGate, 7u, 8, norrath_anttrap_msg_run_HateGateParticle_bytes);
    return failures ? 1 : 0;
}
