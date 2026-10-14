"""EE object check for g_pUserRebootBuf."""

import unittest

import support

SOURCE = support.SOURCES / "kernel-patches-g-user-reboot-buf/g_p_user_reboot_buf.cpp"


class ObjectTests(support.KernelPatchTest):
    def test_object(self):
        data, sections, symbols, relocations = self.expect_object(
            SOURCE, 'g_pUserRebootBuf', 4, content=b'\x00\x00\x00\x00',
            imports=[])
        row = support.defined(symbols, 'g_pUserRebootBuf')
        self.assertEqual(sections[row["shndx"]]["name"], '.bss')
        found = sorted((item["symbol"], item["offset"] - row["value"]) for item in relocations
                       if row["value"] <= item["offset"] < row["value"] + 4)
        self.assertEqual(found, sorted([]))
        for _name, offset in found:
            self.assertEqual(support.object_bytes(data, sections, row)[offset:offset + 4], bytes(4))


if __name__ == "__main__":
    unittest.main()
