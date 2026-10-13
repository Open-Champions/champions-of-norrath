// Observed Init__12VIWndOptionsP7VIScene at 0x010c62c8, 156 bytes. With the window as this and
// the scene as the second argument, in this order:
//   store 0 at this + 1180, 0 at this + 36, and 0 at the file-local word sBattleMusicText
//   (0x011e8bd0, published as `VIWndOptions$sBattleMusicText`);
//   Init__13VIBattleMusicP7VIScene(this + 1200, scene);
//   store the single-precision value 0.02f (0x3ca3d70a) at this + 1216;
//   if the word at this + 1196 is nonzero: RemoveChild__5VIWndP5VIWnd(this, that word) and store 0 there;
//   Init__12VIWndMessage(this + 1232); store 0 at this + 1260; AddChild__5VIWndP5VIWnd(this, this + 1232).
// The result is zero. The word at this + 1196 is a child window pointer by inference from the
// RemoveChild call; the meaning of the other stores is unknown.
#if defined(NORRATH_HOST_CONTRACT)
extern "C" char* norrath_host_translate(unsigned address);
#define NORRATH_TRANSLATE(address) norrath_host_translate(address)
#else
#define NORRATH_TRANSLATE(address) reinterpret_cast<char*>(static_cast<unsigned long>(address))
#endif

extern "C" {
struct VIScene;
extern int norrath_options_battle_music_text __asm__("VIWndOptions$sBattleMusicText");

void Init__13VIBattleMusicP7VIScene(void* music, VIScene* scene);
void RemoveChild__5VIWndP5VIWnd(void* window, void* child);
void Init__12VIWndMessage(void* message);
void AddChild__5VIWndP5VIWnd(void* window, void* child);

int Init__12VIWndOptionsP7VIScene(void* self, VIScene* scene) {
    char* base = static_cast<char*>(self);
    *reinterpret_cast<int*>(base + 1180) = 0;
    *reinterpret_cast<int*>(base + 36) = 0;
    norrath_options_battle_music_text = 0;
    Init__13VIBattleMusicP7VIScene(base + 1200, scene);
    *reinterpret_cast<float*>(base + 1216) = 0.02f;
    unsigned child = *reinterpret_cast<unsigned*>(base + 1196);
    if (child) {
        RemoveChild__5VIWndP5VIWnd(self, NORRATH_TRANSLATE(child));
        *reinterpret_cast<unsigned*>(base + 1196) = 0;
    }
    Init__12VIWndMessage(base + 1232);
    *reinterpret_cast<int*>(base + 1260) = 0;
    AddChild__5VIWndP5VIWnd(self, base + 1232);
    return 0;
}
}
