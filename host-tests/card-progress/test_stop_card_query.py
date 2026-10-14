"""stopCardQuery resets the card query and clears a finished async mode."""

import unittest

import support

SOURCE = support.SOURCES / "card-progress-stop-card-query/stop_card_query.cpp"
IMPORTS = [
    "MCA_Cmd__Fiiii", "MCA_FileOpen", "MCA_Reset__Fv", "MCA_Sync__FPi",
    "asyncMode", "errorCondition", "syncState",
]


class StopCardQueryTests(support.CardTest):
    def test_ee_imports(self):
        data, sections, symbols, _relocations = support.compile_ee(SOURCE)
        row = support.defined(symbols, "stopCardQuery__Fv")
        self.assertGreater(row["size"], 0)
        self.assertGreaterEqual(row["info"] >> 4, 1)
        self.assertEqual(support.undefined(symbols), IMPORTS)

    def test_closed_file_keeps_sync_state(self):
        self.host([SOURCE], r'''
extern "C" int MCA_FileOpen = 0;
extern "C" int asyncMode = 5;
extern "C" int syncState = 9;
extern "C" int errorCondition = 3;
static int resets, commands, syncs;
extern "C" void MCA_Reset__Fv() { resets += 1; MCA_FileOpen = 0; }
extern "C" void MCA_Cmd__Fiiii(int, int, int, int) { commands += 1; }
extern "C" void MCA_Sync__FPi(int*) { syncs += 1; }
extern "C" int stopCardQuery__Fv();
int main() {
    if (stopCardQuery__Fv() != ~5) return 1;
    if (resets != 1 || commands != 0 || syncs != 0) return 2;
    if (errorCondition != 0 || syncState != 9 || asyncMode != 5) return 3;
    return 0;
}
''')

    def test_negative_mode_clears_sync_state(self):
        self.host([SOURCE], r'''
extern "C" int MCA_FileOpen = 0;
extern "C" int asyncMode = -1;
extern "C" int syncState = 9;
extern "C" int errorCondition = 4;
extern "C" void MCA_Reset__Fv() {}
extern "C" void MCA_Cmd__Fiiii(int, int, int, int) { }
extern "C" void MCA_Sync__FPi(int*) { }
extern "C" int stopCardQuery__Fv();
int main() {
    if (stopCardQuery__Fv() != 0) return 1;
    if (syncState != 0 || errorCondition != 0) return 2;
    return 0;
}
''')

    def test_open_file_reads_words_after_sync(self):
        self.host([SOURCE], r'''
extern "C" int MCA_FileOpen = 0;
extern "C" int asyncMode = 1;
extern "C" int syncState = 2;
extern "C" int errorCondition = 3;
static int command, text, third, fourth, order;
extern "C" void MCA_Reset__Fv() {
    if (order != 0) MCA_FileOpen = -1;
    order = 1;
    MCA_FileOpen = 7;
}
extern "C" void MCA_Cmd__Fiiii(int a, int b, int c, int d) {
    if (order != 1) command = -1;
    order = 2;
    command = a; text = b; third = c; fourth = d;
}
extern "C" void MCA_Sync__FPi(int* error) {
    if (order != 2 || error != &errorCondition) order = -1;
    order = 3;
    *error = 77;
    asyncMode = 4;
    syncState = 8;
}
extern "C" int stopCardQuery__Fv();
int main() {
    if (stopCardQuery__Fv() != ~4) return 1;
    if (order != 3 || command != 1 || text != 0 || third != 0 || fourth != 0) return 2;
    if (errorCondition != 0 || syncState != 8 || asyncMode != 4) return 3;
    return 0;
}
''')

    def test_sync_can_finish_the_mode(self):
        self.host([SOURCE], r'''
extern "C" int MCA_FileOpen = 1;
extern "C" int asyncMode = 3;
extern "C" int syncState = 6;
extern "C" int errorCondition = 1;
extern "C" void MCA_Reset__Fv() {}
extern "C" void MCA_Cmd__Fiiii(int, int, int, int) {}
extern "C" void MCA_Sync__FPi(int* error) {
    *error = 77;
    asyncMode = -1;
    syncState = 6;
}
extern "C" int stopCardQuery__Fv();
int main() {
    if (stopCardQuery__Fv() != 0) return 1;
    if (syncState != 0 || errorCondition != 0 || asyncMode != -1) return 2;
    return 0;
}
''')


if __name__ == "__main__":
    unittest.main()
