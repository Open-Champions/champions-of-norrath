// Observed: AddPlay__9VIHSpriteiR7VIScene at 0x01063a78, 84 bytes. Arguments (this, id, scene). It calls Animation__7VIScenei(scene, id);
// with the returned record r it tail-calls AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(this, id, the word at r+32, the float at r+28,
// scene) and returns that call's result.
// Inferred: this overload looks up the scene's animation record by id and forwards that record's playback type (+32) and a float (+28) to
// the full overload. The meaning of the float is not established.
// Unknown: the type of the record returned by Animation__7VIScenei beyond these two fields.
extern "C" {
unsigned char* Animation__7VIScenei(void* scene, int id);
int AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(void* object, int id, int playback, float value, void* scene);

int AddPlay__9VIHSpriteiR7VIScene(void* object, int id, void* scene) {
    unsigned char* animation = Animation__7VIScenei(scene, id);
    return AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(object, id, *reinterpret_cast<int*>(animation + 32), *reinterpret_cast<float*>(animation + 28),
                                                          scene);
}
}
