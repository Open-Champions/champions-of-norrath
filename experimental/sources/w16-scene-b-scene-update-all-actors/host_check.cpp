#include <initializer_list>
#include <vector>
static std::vector<int> updated, nexts;
static int begin_value, end_value, fail_at;
static unsigned char* expected_scene;
extern "C" int Begin__Ct6VIList1Z7VIActor(void*) { return begin_value; }
extern "C" int End__Ct6VIList1Z7VIActor(const void*) { return end_value; }
extern "C" int Next__Ct6VIList1Z7VIActori(void*, int item) { nexts.push_back(item); return item + 1; }
extern "C" int UpdateActor__7VIScenei(unsigned char* scene, int actor) {
    if (scene != expected_scene) updated.push_back(-1);
    updated.push_back(actor);
    return actor == fail_at ? -5 : 0;
}
#include "update_all_actors.cpp"

int main() {
    unsigned char scene[2000];
    expected_scene = scene;
    for (int n : {0, 1, 4}) {
        for (int fail : {-100, 10, 12}) {
            updated.clear();
            nexts.clear();
            begin_value = 10;
            end_value = 10 + n;
            fail_at = fail;
            int r = UpdateAllActors__7VIScene(scene);
            int stop = (fail >= 10 && fail < 10 + n) ? fail - 10 + 1 : n;
            bool failed = fail >= 10 && fail < 10 + n;
            if (r != (failed ? -1 : 0)) return 1;
            if (static_cast<int>(updated.size()) != stop) return 2;
            if (static_cast<int>(nexts.size()) != (failed ? stop - 1 : n)) return 3;
            for (int i = 0; i < stop; ++i)
                if (updated[i] != 10 + i) return 4;
        }
    }
    return 0;
}
