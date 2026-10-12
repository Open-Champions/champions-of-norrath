#include "storage.hpp"
int main() {
    if (sizeof(dialogActor) != 4) return 1;
    if (dialogActor != 0) return 2;
    return 0;
}
