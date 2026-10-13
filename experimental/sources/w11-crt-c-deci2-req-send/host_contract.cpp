// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp deci2_req_send.cpp
extern "C" {
static int calls, g_request, g_words[2], g_result;
int Deci2Call(int request, void* words) {
    ++calls; g_request = request;
    g_words[0] = static_cast<const int*>(words)[0];
    g_words[1] = static_cast<const int*>(words)[1];
    return g_result;
}
int sceDeci2ReqSend(int socket, int destination);
}
int main() {
    g_result = 11;
    if (sceDeci2ReqSend(2, 72) != 11 || calls != 1 || g_request != 3 || g_words[0] != 2 || g_words[1] != 72) return 1;
    sceDeci2ReqSend(-3, 0x1ff);
    if (g_words[0] != -3 || g_words[1] != -1) return 2;       // low byte, sign-extended
    sceDeci2ReqSend(7, 0x180);
    if (g_words[1] != -128) return 3;
    sceDeci2ReqSend(7, 0x17f);
    if (g_words[1] != 127) return 4;
    g_result = -1;
    if (sceDeci2ReqSend(0, 0) != -1 || g_words[1] != 0) return 5;
    return 0;
}
