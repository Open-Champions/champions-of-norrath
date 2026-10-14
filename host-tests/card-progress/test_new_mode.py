"""newMode saves the previous mode only when its second argument is zero."""

import unittest
from pathlib import Path

import support

SOURCE = support.SOURCES / "card-progress-new-mode/new_mode.cpp"


class NewModeTests(support.CardTest):
    def test_ee_imports(self):
        _data, _sections, symbols, _relocations = support.compile_ee(SOURCE)
        row = support.defined(symbols, "newMode__Fii")
        self.assertGreater(row["size"], 0)
        self.assertGreaterEqual(row["info"] >> 4, 1)
        self.assertEqual(support.undefined(symbols), ["asyncMode", "oldAsyncMode", "syncState"])

    def test_zero_argument_saves_previous_mode(self):
        self.host([SOURCE], r'''
extern "C" int asyncMode = 6;
extern "C" int oldAsyncMode = 12345;
extern "C" int syncState = 9;
extern "C" int newMode__Fii(int mode, int argument);
int main() {
    if (newMode__Fii(2, 0) != 6) return 1;
    if (oldAsyncMode != 6 || asyncMode != 2 || syncState != 0) return 2;
    if (newMode__Fii(-1, 0) != 2) return 3;
    if (oldAsyncMode != 2 || asyncMode != -1 || syncState != 0) return 4;
    return 0;
}
''')

    def test_nonzero_argument_leaves_old_mode(self):
        self.host([SOURCE], r'''
extern "C" int asyncMode = 6;
extern "C" int oldAsyncMode = 12345;
extern "C" int syncState = 9;
extern "C" int newMode__Fii(int mode, int argument);
int main() {
    newMode__Fii(2, 1);
    if (oldAsyncMode != 12345 || asyncMode != 2 || syncState != 0) return 1;
    asyncMode = 4;
    oldAsyncMode = 8;
    syncState = 3;
    newMode__Fii(11, -5);
    if (oldAsyncMode != 8 || asyncMode != 11 || syncState != 0) return 2;
    return 0;
}
''')


if __name__ == "__main__":
    unittest.main()
