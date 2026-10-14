// Host comparison only. Not an EE build input.
#include <cstdio>
#include <vector>

static std::vector<int> events;
static int exit_status = -1;

extern "C" unsigned char* norrath_factory_impure_pointer_word;
extern "C" void _exit(int status) { exit_status = status; events.push_back(-100); }
#define exit w8_exit_under_test
#include "exit.cpp"
#undef exit

template <int N> static void fn() { events.push_back(N); }
static NorrathExitReent* captured;
static void hook(NorrathExitReent* reent) { captured = reent; events.push_back(-50); }

static NorrathExitReent reent_image;
unsigned char* norrath_factory_impure_pointer_word = reinterpret_cast<unsigned char*>(&reent_image);

static int expect(const std::vector<int>& want, int status, const char* name) {
    if (events != want || exit_status != status) {
        std::printf("%s: events differ or status %d\n", name, exit_status);
        for (int e : events) std::printf(" %d", e);
        std::printf("\n");
        return 1;
    }
    return 0;
}

int main() {
    int failures = 0;

    // Empty list, no hook: only _exit runs with the status.
    reent_image = NorrathExitReent{};
    events.clear(); exit_status = -1;
    w8_exit_under_test(7);
    failures += expect({-100}, 7, "empty");

    // Two nodes: each walked last-to-first, nodes head-to-tail, then hook, then _exit.
    static NorrathExitNode second{};
    second.count = 1; second.functions[0] = fn<30>;
    reent_image = NorrathExitReent{};
    reent_image.first_node.count = 3;
    reent_image.first_node.functions[0] = fn<10>;
    reent_image.first_node.functions[1] = fn<11>;
    reent_image.first_node.functions[2] = fn<12>;
    reent_image.first_node.next = &second;
    reent_image.list_head = &reent_image.first_node;
    reent_image.hook = hook;
    events.clear(); exit_status = -1; captured = nullptr;
    w8_exit_under_test(-3);
    failures += expect({12, 11, 10, 30, -50, -100}, -3, "two nodes");
    if (captured != &reent_image) { std::printf("hook argument wrong\n"); ++failures; }

    // A node whose count is zero is skipped without calls; the next is still visited.
    static NorrathExitNode tail{};
    tail.count = 1; tail.functions[0] = fn<41>;
    static NorrathExitNode empty{};
    empty.next = &tail;
    reent_image = NorrathExitReent{};
    reent_image.list_head = &empty;
    events.clear(); exit_status = -1;
    w8_exit_under_test(0);
    failures += expect({41, -100}, 0, "empty node");

    if (failures) return 1;
    std::printf("exit cases passed\n");
    return 0;
}
