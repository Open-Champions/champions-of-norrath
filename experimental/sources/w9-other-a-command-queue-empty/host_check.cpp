#include "command_queue_empty.cpp"
int queueStart;
int queueEnd;
int main() {
    if (commandQueueEmpty__Fv() != 1) return 1;
    queueStart = 3;
    if (commandQueueEmpty__Fv() != 0) return 2;
    queueEnd = 3;
    if (commandQueueEmpty__Fv() != 1) return 3;
    queueEnd = 0;
    queueStart = -1;
    if (commandQueueEmpty__Fv() != 0) return 4;
    return 0;
}
