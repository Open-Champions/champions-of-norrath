// Observed modelSetRelightAttachment__FG6Point3 at 0x0016cbf8, extent 80. The Point3
// argument arrives as a pointer to a caller copy. The function sets bit 6 (0x40) of
// modelOption (0x0048408c) with an OR, then copies the twelve bytes of the point into
// attachmentLightPos (0x006e1d50). The order is: read modelOption, copy the point to a
// stack temporary, store modelOption, copy the temporary to attachmentLightPos.
// Returns nothing. modelResetOptionalParams clears modelOption but not the position.
// Inferred: the name says the point is where an attachment is lit from. Unknown: the
// consumer of bit 6 and of attachmentLightPos; both are owned by other packages.
#include "../w9-render-math-b-common/math_types.hpp"

extern "C" {
extern int modelOption;
extern Point3 attachmentLightPos;
void modelSetRelightAttachment__FG6Point3(Point3 point) {
    Point3 copy = point;
    modelOption |= 0x40;
    attachmentLightPos = copy;
}
}
