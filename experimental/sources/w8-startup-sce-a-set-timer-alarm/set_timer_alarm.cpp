/* Observed SetTimerAlarm at 0x002df2b8, 300 bytes, and its local handler at
 * 0x002df258, 92 bytes. Alarm records are 16-byte nodes from a free list whose
 * head is the local word g_pFreeAlarm (0x008e4540); _InitAlarm at 0x002df208
 * chains 64 of them from 0x008e4140. Word 0 is the next link, word 1 the
 * counter, word 2 the caller's handler, word 3 the caller's argument.
 *
 * A null handler returns 0x80000016. With interrupts disabled around the body
 * (DIntr's result decides whether EIntr runs), an empty free list returns
 * 0x80008005. AllocTimerCounter below zero pushes the node back, zeroes its
 * counter word and returns that status. Otherwise the node records handler and
 * argument, SetTimerHandler gets the counter, the 64-bit clock count, the local
 * handler and the node, StartTimerCounter starts the counter, and the result is
 * (node address << 4) | (counter & 0xfe) | 1.
 *
 * The local handler masks the counter it receives to 0xfe, rebuilds that same
 * identifier, calls the caller's handler with it as the first argument and the
 * stored argument as the fourth, and leaves arguments two and three as received.
 * When the caller's handler returns zero, the node is pushed on the free list,
 * its counter word is zeroed, and -1 is returned; otherwise the handler's
 * result is returned. What the timer service does with those results is unknown.
 * Compiled output is not claimed to match the original.
 */
extern "C" {
int DIntr();
int EIntr();
int AllocTimerCounter();
int SetTimerHandler(int counter, unsigned long long clocks,
                    int (*handler)(int, int, int, void*), void* argument);
int StartTimerCounter(int counter);
}

struct AlarmNode;
extern "C" AlarmNode* g_pFreeAlarm;

struct AlarmNode {
    AlarmNode* next;
    int counter;
    int (*handler)(int, int, int, int);
    int argument;
};

static int alarm_id(const AlarmNode* node, int counter) {
    return static_cast<int>((static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(node)) << 4) |
                            (static_cast<unsigned>(counter) & 0xfeu) | 1u);
}

static void release(AlarmNode* node) {
    node->next = g_pFreeAlarm;
    g_pFreeAlarm = node;
    node->counter = 0;
}

static int alarm_handler(int counter, int second, int third, void* record) {
    AlarmNode* node = static_cast<AlarmNode*>(record);
    const int result = node->handler(alarm_id(node, counter & 0xfe), second, third, node->argument);
    if (result != 0) return result;
    release(node);
    return -1;
}

extern "C" int SetTimerAlarm(unsigned long long clocks, int (*handler)(int, int, int, int), int argument) {
    if (handler == nullptr) return static_cast<int>(0x80000016u);
    const int was_enabled = DIntr();
    AlarmNode* node = g_pFreeAlarm;
    if (node != nullptr) g_pFreeAlarm = node->next;
    if (node == nullptr) {
        if (was_enabled != 0) EIntr();
        return static_cast<int>(0x80008005u);
    }
    const int counter = AllocTimerCounter();
    if (counter < 0) {
        release(node);
        if (was_enabled != 0) EIntr();
        return counter;
    }
    node->handler = handler;
    node->argument = argument;
    node->counter = counter;
    SetTimerHandler(counter, clocks, alarm_handler, node);
    StartTimerCounter(counter);
    if (was_enabled != 0) EIntr();
    return alarm_id(node, counter);
}
