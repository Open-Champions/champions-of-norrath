"""EE object check for g_pFreeAlarm."""

import unittest

import support

SOURCE = support.SOURCES / "kernel-patches-g-p-free-alarm/g_p_free_alarm.cpp"


class ObjectTests(support.KernelPatchTest):
    def test_object(self):
        data, sections, symbols, relocations = self.expect_object(
            SOURCE, 'g_pFreeAlarm', 4, content=bytes(4),
            imports=[])
        row = support.defined(symbols, 'g_pFreeAlarm')
        self.assertEqual(sections[row["shndx"]]["name"], '.bss')
        found = sorted((item["symbol"], item["offset"] - row["value"]) for item in relocations
                       if row["value"] <= item["offset"] < row["value"] + 4)
        self.assertEqual(found, sorted([]))
        for _name, offset in found:
            self.assertEqual(support.object_bytes(data, sections, row)[offset:offset + 4], bytes(4))


if __name__ == "__main__":
    unittest.main()
