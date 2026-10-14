"""EE object check for g_AlarmBuf."""

import unittest

import support

SOURCE = support.SOURCES / "kernel-patches-g-alarm-buf/g_alarm_buf.cpp"


class ObjectTests(support.KernelPatchTest):
    def test_object(self):
        data, sections, symbols, relocations = self.expect_object(
            SOURCE, 'g_AlarmBuf', 1024, content=bytes(1024),
            imports=[])
        row = support.defined(symbols, 'g_AlarmBuf')
        self.assertEqual(sections[row["shndx"]]["name"], '.bss')
        found = sorted((item["symbol"], item["offset"] - row["value"]) for item in relocations
                       if row["value"] <= item["offset"] < row["value"] + 4)
        self.assertEqual(found, sorted([]))
        for _name, offset in found:
            self.assertEqual(support.object_bytes(data, sections, row)[offset:offset + 4], bytes(4))


if __name__ == "__main__":
    unittest.main()
