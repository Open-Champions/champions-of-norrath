#include "../w11-rend-a-common/rend.hpp"
// Observed lightAdd__FP5Light at 0x0013d230, 200 bytes. The argument is a light record. Words used:
// +0 next light, +4 previous link, and the floats at +48 and +52 (x and y).
//  1. Unlink: when the previous link is nonzero the word it points at (which is the previous light's
//     +0, or a list head word) takes the light's next; then when next is nonzero its +4 takes the
//     light's previous link. Next and previous link are then cleared.
//  2. gx = ((int)x + 11520) >> 8 and gy = ((int)y + 11520) >> 8 (truncating conversion, arithmetic
//     shift). With gx below 90 (unsigned) and 0 <= gy < 90 the head word is lightGrid word gy * 90 + gx;
//     otherwise it is the head word at 0x00486848, the first of the two symbols named offTheGrid.
//  3. Push at the front: previous link = head word address, next = old first, head word = light, and
//     the old first (when nonzero) gets previous link = light.
// Nothing is returned.
// Inferred: lights are kept in a 90 by 90 grid of 256-unit cells, with an overflow list. Unknown: the
// grid origin's meaning, and the light record beyond these offsets. Nothing here was executed on the EE.
extern "C" {
extern unsigned char lightGrid[90 * 90 * 4];
extern unsigned char offTheGrid[8];
void lightAdd__FP5Light(unsigned char* light) {
    using namespace rend;
    unsigned char* previous = ptr(light, 4);
    if (previous != nullptr) at<u32>(previous, 0) = at<u32>(light, 0);
    unsigned char* following = ptr(light, 0);
    if (following != nullptr) at<u32>(following, 4) = at<u32>(light, 4);
    at<u32>(light, 4) = 0;
    at<u32>(light, 0) = 0;
    const int gx = (static_cast<int>(at<f32>(light, 48)) + 11520) >> 8;
    const int gy = (static_cast<int>(at<f32>(light, 52)) + 11520) >> 8;
    unsigned char* head;
    if (static_cast<unsigned>(gx) < 90u && gy >= 0 && gy < 90) head = lightGrid + (gy * 90 + gx) * 4;
    else head = offTheGrid;
    set_ptr(light, 4, head);
    at<u32>(light, 0) = at<u32>(head, 0);
    set_ptr(head, 0, light);
    unsigned char* first = ptr(light, 0);
    if (first != nullptr) set_ptr(first, 4, light);
}
}
