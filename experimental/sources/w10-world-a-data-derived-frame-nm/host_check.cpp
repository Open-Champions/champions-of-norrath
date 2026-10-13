#include "derived_frame_nm.cpp"
#include <cstring>
int main() {
    if (sizeof(derivedFrameNm) != 4) return 1;
    if (derivedFrameNm != -10) return 2;
    return 0;
}
