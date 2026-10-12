"""sceMcSync polls one semaphore and signals the other when the command ends."""

import unittest

import support

SOURCE = support.SOURCES / "card-progress-sce-mc-sync/sce_mc_sync.cpp"
IMPORTS = [
    "PollSema", "SignalSema", "WaitSema", "mcRunCmdNo", "retval",
    "semaidRegFunc", "semaid_timerfunc",
]


class SceMcSyncTests(support.CardTest):
    def test_ee_imports(self):
        _data, _sections, symbols, _relocations = support.compile_ee(SOURCE)
        row = support.defined(symbols, "sceMcSync")
        self.assertGreater(row["size"], 0)
        self.assertGreaterEqual(row["info"] >> 4, 1)
        self.assertEqual(support.undefined(symbols), IMPORTS)

    def test_idle_command_returns_without_calls(self):
        self.host([SOURCE], r'''
extern "C" int mcRunCmdNo = 0;
extern "C" int semaidRegFunc = 11;
extern "C" int semaid_timerfunc = 22;
extern "C" int retval = 33;
static int polls, waits, signals;
extern "C" int PollSema(int) { polls += 1; return 0; }
extern "C" int WaitSema(int) { waits += 1; return 0; }
extern "C" int SignalSema(int) { signals += 1; return 0; }
extern "C" int sceMcSync(int mode, int* command, int* result);
int main() {
    int command = 5, result = 6;
    if (sceMcSync(0, &command, &result) != -1) return 1;
    if (polls || waits || signals) return 2;
    if (command != 5 || result != 6 || mcRunCmdNo != 0) return 3;
    if (sceMcSync(1, 0, 0) != -1) return 4;
    return 0;
}
''')

    def test_nonzero_mode_waits_only_when_the_poll_fails(self):
        self.host([SOURCE], r'''
extern "C" int mcRunCmdNo = 7;
extern "C" int semaidRegFunc = 11;
extern "C" int semaid_timerfunc = 22;
extern "C" int retval = 33;
static int polls, waits, signals, poll_id;
extern "C" int PollSema(int id) { polls += 1; poll_id = id; return -4; }
extern "C" int WaitSema(int) { waits += 1; return 0; }
extern "C" int SignalSema(int) { signals += 1; return 0; }
extern "C" int sceMcSync(int mode, int* command, int* result);
int main() {
    int command = 1, result = 2;
    if (sceMcSync(1, &command, &result) != 0) return 1;
    if (polls != 1 || poll_id != 22 || waits != 0 || signals != 0) return 2;
    if (command != 7 || result != 2 || mcRunCmdNo != 7) return 3;
    return 0;
}
''')

    def test_completed_poll_stores_result_and_signals(self):
        self.host([SOURCE], r'''
extern "C" int mcRunCmdNo = 7;
extern "C" int semaidRegFunc = 11;
extern "C" int semaid_timerfunc = 22;
extern "C" int retval = 33;
static int waits, signal_id, command_at_signal;
extern "C" int PollSema(int id) { return id == 22 ? 0 : -1; }
extern "C" int WaitSema(int) { waits += 1; return -5; }
extern "C" int SignalSema(int id) {
    signal_id = id;
    command_at_signal = mcRunCmdNo;
    return 0;
}
extern "C" int sceMcSync(int mode, int* command, int* result);
int main() {
    int command = 1, result = 2;
    if (sceMcSync(4, &command, &result) != 1) return 1;
    if (waits != 0 || signal_id != 11 || command_at_signal != 0) return 2;
    if (command != 7 || result != 33 || mcRunCmdNo != 0) return 3;
    mcRunCmdNo = 9;
    retval = 44;
    if (sceMcSync(1, 0, 0) != 1 || mcRunCmdNo != 0) return 4;
    mcRunCmdNo = 6;
    waits = 0;
    command = 0;
    result = 0;
    if (sceMcSync(0, &command, &result) != 1 || waits != 0) return 5;
    if (command != 6 || result != 44 || mcRunCmdNo != 0 || signal_id != 11) return 6;
    return 0;
}
''')

    def test_zero_mode_waits_once_then_completes(self):
        self.host([SOURCE], r'''
extern "C" int mcRunCmdNo = 7;
extern "C" int semaidRegFunc = 11;
extern "C" int semaid_timerfunc = 22;
extern "C" int retval = 33;
static int polls, waits, wait_id, signals;
extern "C" int PollSema(int) { polls += 1; return -1; }
extern "C" int WaitSema(int id) { waits += 1; wait_id = id; return -5; }
extern "C" int SignalSema(int) { signals += 1; return 0; }
extern "C" int sceMcSync(int mode, int* command, int* result);
int main() {
    int command = 0, result = 0;
    if (sceMcSync(0, &command, &result) != 1) return 1;
    if (polls != 1 || waits != 1 || wait_id != 22 || signals != 1) return 2;
    if (command != 7 || result != 33 || mcRunCmdNo != 0) return 3;
    mcRunCmdNo = 8;
    polls = waits = signals = 0;
    if (sceMcSync(0, 0, 0) != 1) return 4;
    if (polls != 1 || waits != 1 || signals != 1 || mcRunCmdNo != 0) return 5;
    return 0;
}
''')


if __name__ == "__main__":
    unittest.main()
