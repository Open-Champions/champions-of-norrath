// Observed: Init__6VIBBoxiP7VIVect3 at 0x01072410 is 112 bytes. Arguments: box (a0), count
// (a1) and an array of 12-byte vectors (a2). For i from 0 while i < count it calls
// Bound__6VIBBoxiRC7VIVect3(box, i == 0, &points[i]) and the function returns 0. With
// count <= 0 nothing is called. The first call passes 1 and every later call passes 0.
// Inferred: the box is built by growing it over each point, the first flag meaning "start a
// new box". Unknown: the meaning of the flag in Bound__6VIBBoxiRC7VIVect3, and the box
// layout (not touched here).
extern "C" {
void Bound__6VIBBoxiRC7VIVect3(void* box, int first, const void* point);

int Init__6VIBBoxiP7VIVect3(void* box, int count, const unsigned char* points) {
    for (int i = 0; i < count; ++i) {
        Bound__6VIBBoxiRC7VIVect3(box, i == 0, points + i * 12);
    }
    return 0;
}
}
