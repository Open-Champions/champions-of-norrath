// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp deci2_poll.cpp
extern "C" {
static int calls, g_request, g_word, g_result;
int Deci2Call(int request, void* words) {
    ++calls; g_request = request; g_word = *static_cast<const int*>(words);
    return g_result;
}
int sceDeci2Poll(int socket);
}
int main() {
    g_result = 9;
    if (sceDeci2Poll(3) != 9 || calls != 1 || g_request != 4 || g_word != 3) return 1;
    g_result = -2;
    if (sceDeci2Poll(-6) != -2 || g_word != -6 || calls != 2) return 2;
    return 0;
}
