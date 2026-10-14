"""EE object check for kernel_alarm_sysentry."""

import unittest

import support

SOURCE = support.SOURCES / "kernel-patches-alarm-sysentry/alarm_sysentry.cpp"


class ObjectTests(support.KernelPatchTest):
    def test_object(self):
        data, sections, symbols, relocations = self.expect_object(
            SOURCE, 'kernel_alarm_sysentry', 64, content=b'Z\x00\x00\x00\x00\x00\x00\x00[\x00\x00\x00\x00`\x07\x80\xfc\x00\x00\x00\x00\x00\x00\x00\xfe\x00\x00\x00\x00\x00\x00\x00\xfd\x00\x00\x00\x00\x00\x00\x00\xff\x00\x00\x00\x00\x00\x00\x00,\x01\x00\x00\x00\x00\x00\x00\x08\x00\x00\x00\x00\x00\x00\x00',
            imports=['kernel_alarm_pattern_kcopy'])
        row = support.defined(symbols, 'kernel_alarm_sysentry')
        self.assertEqual(sections[row["shndx"]]["name"], '.data')
        found = sorted((item["symbol"], item["offset"] - row["value"]) for item in relocations
                       if row["value"] <= item["offset"] < row["value"] + 64)
        self.assertEqual(found, sorted([('kernel_alarm_pattern_kcopy', 4)]))
        for _name, offset in found:
            self.assertEqual(support.object_bytes(data, sections, row)[offset:offset + 4], bytes(4))


if __name__ == "__main__":
    unittest.main()
