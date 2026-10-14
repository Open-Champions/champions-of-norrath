#include <initializer_list>
#include <vector>
static std::vector<int> removed, nexts;
static int begin_value, end_value;
extern "C" int Begin__Ct6VIList1Z7VIActor(void*) { return begin_value; }
extern "C" int End__Ct6VIList1Z7VIActor(const void*) { return end_value; }
extern "C" int Next__Ct6VIList1Z7VIActori(void*, int item) { nexts.push_back(item); return item + 1; }
static unsigned char* expected_scene;
extern "C" int RemoveActor__7VIScenei(unsigned char* scene, int actor) {
    if (scene != expected_scene) removed.push_back(-1);
    removed.push_back(actor);
    return 0;
}
#include "remove_all_actors.cpp"

int main() {
    unsigned char scene[2000];
    expected_scene = scene;
    for (int n : {0, 1, 4}) {
        removed.clear();
        nexts.clear();
        begin_value = 10;
        end_value = 10 + n;
        if (RemoveAllActors__7VIScene(scene) != 0) return 1;
        if (static_cast<int>(removed.size()) != n || nexts.size() != removed.size()) return 2;
        for (int i = 0; i < n; ++i)
            if (removed[i] != 10 + i || nexts[i] != 10 + i) return 3;
    }
    return 0;
}
