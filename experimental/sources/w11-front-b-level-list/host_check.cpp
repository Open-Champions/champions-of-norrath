// Host check for list.1073. Not an EE plan input.
// On the 64-bit host the record is wider; the 60-byte size is a target fact, so the offsets are
// checked with offsetof on the field order and the byte sizes with the member sizes.
#include "level_list.cpp"
#include <cstddef>
#include <cstring>
int main() {
    if (sizeof(levelList) / sizeof(levelList[0]) != 63) return 1;
    if (offsetof(LevelRecord, forWorldDifficulty) != sizeof(void*)) return 2;
    if (sizeof(LevelRecord::audio) != 8 || sizeof(short) != 2) return 3;
    // exactly one record has a null name, and it is the last
    for (int i = 0; i < 62; ++i) if (levelList[i].name == nullptr) return 4;
    if (levelList[62].name != nullptr) return 5;
    // names are unique and non-empty
    for (int i = 0; i < 62; ++i) {
        if (levelList[i].name[0] == 0) return 6;
        for (int j = i + 1; j < 62; ++j) if (std::strcmp(levelList[i].name, levelList[j].name) != 0) continue; else return 7;
    }
    // spot checks against values read from the image
    const LevelRecord& first = levelList[0];
    if (std::strcmp(first.name, "kelethin") != 0 || first.forWorldDifficulty != 1.0f || first.shadowBrightness != 2.0f ||
        first.megatileRamkmax != 0x2800 || first.deathHeight != -100000.0f || first.minCamHeight != -100000.0f ||
        first.maxCamHeight != 100000.0f || first.weatherArg != 0 || first.waterPatchesArg != 0 || first.groundType != 4 ||
        first.tileEntityLightScale != 1.0f || first.levelFlags != 0x20000 || first.numEnvironments != 2) return 8;
    const LevelRecord& gob = levelList[4];
    if (std::strcmp(gob.name, "goblin1a_0") != 0 || gob.forWorldDifficulty != 2.0f || gob.audio[0] != 5 ||
        gob.audio[1] != 6745 || gob.levelFlags != 0x1020000 || gob.numEnvironments != 0) return 9;
    if (std::strcmp(levelList[61].name, "hate4") != 0 || levelList[61].forWorldDifficulty != 20.0f) return 10;
    if (std::strcmp(levelList[23].name, "lava1") != 0 || levelList[23].waterPatchesArg != 78 || levelList[23].levelFlags != 0x1011000) return 11;
    const LevelRecord& tail = levelList[62];
    if (tail.waterPatchesArg != 0x30 || tail.levelFlags != 0x20000 || tail.numEnvironments != 0) return 12;
    return 0;
}
