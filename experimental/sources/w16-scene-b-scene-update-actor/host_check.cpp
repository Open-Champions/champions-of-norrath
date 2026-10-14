// Host check only; not a build input. Words hold low 32 bits of arena addresses (see NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "update_actor.cpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>
alignas(16) static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }

static std::vector<int> order;
static int remove_result, insert_result;
static unsigned char *calc_actor, *mul_matrix, *mul_in, *mul_out, *tr_box, *tr_matrix, *rm_actor_scene;
static int rm_actor;
static InsertRecord snapshot;
static void* insert_world;
extern "C" void CalcObjectToWorld__7VIActor(void* actor) { order.push_back(1); calc_actor = static_cast<unsigned char*>(actor); }
extern "C" void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(void* m, const void* in, void* out) {
    order.push_back(2);
    mul_matrix = static_cast<unsigned char*>(m); mul_in = (unsigned char*)in; mul_out = static_cast<unsigned char*>(out);
    float v[3] = {10, 20, 30};
    std::memcpy(out, v, 12);
}
extern "C" int RemoveActor__7VIScenei(unsigned char* scene, int actor) { order.push_back(3); rm_actor_scene = scene; rm_actor = actor; return remove_result; }
extern "C" void Transform__C6VIBBoxRC10VIMatrix44P7VIVect3(const void* box, const void* matrix, void* points) {
    order.push_back(4);
    tr_box = (unsigned char*)box; tr_matrix = (unsigned char*)matrix;
    float* p = static_cast<float*>(points);
    for (int i = 0; i < 24; ++i) p[i] = static_cast<float>(i) + 0.5f;
}
extern "C" int InsertActor__7VIWorldR17VISceneInsertVars(void* world, InsertRecord* record) {
    order.push_back(5); insert_world = world; snapshot = *record;
    return insert_result;
}

int main() {
    bool ok = true;
    for (int rm : {0, -1}) for (int ins : {0, -4}) {
        std::memset(arena, 0, sizeof arena);
        order.clear(); remove_result = rm; insert_result = ins;
        unsigned char* scene = arena;
        unsigned char* table = arena + 0x1000;      // 12-byte entries
        unsigned char* info = arena + 0x1400;
        unsigned char* actor = arena + 0x1800;
        auto W = [](unsigned char* b, int off) -> unsigned& { return *reinterpret_cast<unsigned*>(b + off); };
        auto F = [](unsigned char* b, int off) -> float& { return *reinterpret_cast<float*>(b + off); };
        W(scene, 1004) = word_of(table);
        W(scene, 1348) = 41;
        W(table, 12 * 2) = word_of(info);
        F(info, 44) = 3.0f;
        W(actor, 96) = 2;
        F(actor, 92) = 1.5f;
        int r = UpdateActor__7VIScenei(scene, static_cast<int>(word_of(actor)));
        bool ran_insert = rm >= 0;
        if (r != (rm < 0 ? -1 : (ins < 0 ? -1 : 0))) ok = false;
        std::vector<int> want = rm < 0 ? std::vector<int>{1, 2, 3} : std::vector<int>{1, 2, 3, 4, 5};
        if (order != want) ok = false;
        if (calc_actor != actor || mul_matrix != actor || mul_in != info + 32 || mul_out != actor + 136) ok = false;
        if (F(actor, 148) != 4.5f) ok = false;
        if (rm_actor_scene != scene || rm_actor != static_cast<int>(word_of(actor))) ok = false;
        if (W(scene, 1348) != (ran_insert && ins >= 0 ? 42u : 41u)) ok = false;
        if (ran_insert) {
            if (tr_box != info + 8 || tr_matrix != actor || insert_world != scene + 4) ok = false;
            if (snapshot.mode != 0 || snapshot.slot != -1 || snapshot.actor != word_of(actor) || snapshot.scene != word_of(scene)) ok = false;
            for (int i = 0; i < 8; ++i) for (int j = 0; j < 3; ++j)
                if (snapshot.points[i][j] != static_cast<float>(3 * i + j) + 0.5f) ok = false;
            if (snapshot.sphere[0] != 10 || snapshot.sphere[1] != 20 || snapshot.sphere[2] != 30 || snapshot.sphere[3] != 4.5f) ok = false;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
