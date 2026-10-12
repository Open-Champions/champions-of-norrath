"""EE object check for semaid_timerfunc."""

import unittest

import support

SOURCE = support.SOURCES / "kernel-patches-semaid-timerfunc/semaid_timerfunc.cpp"


class ObjectTests(support.KernelPatchTest):
    def test_object(self):
        data, sections, symbols, relocations = self.expect_object(
            SOURCE, 'semaid_timerfunc', 4, content=b'\xff\xff\xff\xff',
            imports=[])
        row = support.defined(symbols, 'semaid_timerfunc')
        self.assertEqual(sections[row["shndx"]]["name"], '.data')
        found = sorted((item["symbol"], item["offset"] - row["value"]) for item in relocations
                       if row["value"] <= item["offset"] < row["value"] + 4)
        self.assertEqual(found, sorted([]))
        for _name, offset in found:
            self.assertEqual(support.object_bytes(data, sections, row)[offset:offset + 4], bytes(4))


if __name__ == "__main__":
    unittest.main()
