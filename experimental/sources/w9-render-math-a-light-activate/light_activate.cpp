// Observed lightActivate__FP5Light at 0x0013d2f8, 200 bytes. It performs the same list
// relinking as the authored lightAdd (0x13d230): word 0 of a light is the next light and
// word 1 is the address of the slot that points at this light.
//  1. If previous is nonzero, *previous takes next. The next word is then re-read from
//     the light, so a previous that aliases the light itself is honoured.
//  2. If that next is nonzero, next->previous takes previous.
//  3. x (offset 48) and y (offset 52) are truncated to integers; previous and next are
//     cleared.
//  4. gx = (int(x) + 11520) >> 8, gy likewise. gx below 90 unsigned and 0 <= gy < 90 pick
//     the head word lightGrid[gy * 90 + gx] (0x004f40f0, 90 by 90 words); otherwise the
//     head word at 0x00486848 is used.
//  5. The light is pushed at the front: previous = head, next = old first, *head = light,
//     and the old first's previous, when there is one, takes the light.
// Unknown: whether the identical body of lightAdd comes from one source function; the
// two are separate addresses with distinct symbol names. Fresh compilation is not an
// original-byte match.
struct LightLink {
    LightLink* next;
    LightLink** previous;
    unsigned char unknown_8[40];
    float x;
    float y;
};

extern "C" {
extern unsigned char norrath_floor_light_grid_view[];
extern unsigned char norrath_light_off_grid_head_view[];

void lightActivate__FP5Light(LightLink* light) {
    if (light->previous != 0) *light->previous = light->next;
    LightLink* following = light->next;
    if (following != 0) following->previous = light->previous;
    const int x = static_cast<int>(light->x);
    const int y = static_cast<int>(light->y);
    light->previous = 0;
    light->next = 0;
    const int cell_x = (x + 11520) >> 8;
    const int cell_y = (y + 11520) >> 8;
    LightLink** head;
    if (static_cast<unsigned>(cell_x) < 90u && cell_y >= 0 && cell_y < 90) {
        head = reinterpret_cast<LightLink**>(norrath_floor_light_grid_view) + (cell_y * 90 + cell_x);
    } else {
        head = reinterpret_cast<LightLink**>(norrath_light_off_grid_head_view);
    }
    LightLink* first = *head;
    light->previous = head;
    light->next = first;
    *head = light;
    if (light->next != 0) light->next->previous = reinterpret_cast<LightLink**>(light);
}
}
