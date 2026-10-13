#include "fs_sem_init.cpp"

static int created;
static int next_id = 41;
static int seen_words[3];
extern "C" int CreateSema(void* parameters) {
    const int* words = static_cast<const int*>(parameters);
    ++created;
    seen_words[0] = words[1];
    seen_words[1] = words[2];
    seen_words[2] = words[5];
    return next_id;
}

int main() {
    if (_fs_semid != -1) return 1;
    _sceFsSemInit();
    if (created != 1 || _fs_semid != 41) return 2;
    if (seen_words[0] != 1 || seen_words[1] != 1 || seen_words[2] != 0) return 3;
    // Already created: the id is kept and no second semaphore is made.
    next_id = 42;
    _sceFsSemInit();
    if (created != 1 || _fs_semid != 41) return 4;
    // Any value other than -1 counts as created, including 0 and a failure code.
    _fs_semid = 0;
    _sceFsSemInit();
    if (created != 1 || _fs_semid != 0) return 5;
    // -1 again makes a new one, and a negative create result is stored as given.
    _fs_semid = -1;
    next_id = -3;
    _sceFsSemInit();
    if (created != 2 || _fs_semid != -3) return 6;
    return 0;
}
