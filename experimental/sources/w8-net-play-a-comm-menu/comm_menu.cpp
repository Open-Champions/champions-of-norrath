// Observed commMenu at 0x00428960, 96 bytes: six 16-byte entries. Each entry is
// a text key pointer, a kind word, a pointer to a state word, and a zero word.
// Entries 0..4 use the keys menu_commenable, menu_commecho, menu_commrecord,
// menu_commthresh, menu_commplay (strings at 0x46ad00..0x46ad40) with kinds
// 3, 3, 4, 4, 4 and targets commEnable (0x486644), commEcho (0x486640),
// commRecord (0x486630), commThresh (0x486634), commPlayback (0x486638). The
// sixth entry is all zero. What kind 3 and kind 4 select is not established.
extern "C" {
extern int commEnable;
extern int commEcho;
extern float commRecord;
extern float commThresh;
extern float commPlayback;

struct CommMenuEntry {
    const char* key;
    int kind;
    void* target;
    int reserved;
};

CommMenuEntry commMenu[6] = {
    {"menu_commenable", 3, &commEnable, 0},
    {"menu_commecho", 3, &commEcho, 0},
    {"menu_commrecord", 4, &commRecord, 0},
    {"menu_commthresh", 4, &commThresh, 0},
    {"menu_commplay", 4, &commPlayback, 0},
    {0, 0, 0, 0},
};
}
