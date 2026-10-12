#include "sent_end_msg.cpp"
int main() {
    static_assert(sizeof(sentEndMsg) == 4, "one word");
    if (sentEndMsg != 0) return 1;
    sentEndMsg = -3;
    return sentEndMsg == -3 ? 0 : 2;
}
