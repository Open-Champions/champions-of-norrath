// amxClearRecall is the 84-byte native at 0x0013bf38. For each of nmPlayers
// records it copies the five bytes "None" including the terminator onto byte
// 36. Those bytes are the file image at 0x00483a30. The source does not
// advance. A non-positive count returns zero.
extern "C" int nmPlayers;
extern "C" unsigned char player[];
extern "C" int amxClearRecall__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    int count = nmPlayers;
    if (count <= 0) return 0;
    const unsigned char source[5] = {'N', 'o', 'n', 'e', 0};
    unsigned char* destination = player + 36;
    do {
        for (int index = 0; index < 5; ++index) destination[index] = source[index];
        destination += 23984;
        count -= 1;
    } while (count != 0);
    return 0;
}
