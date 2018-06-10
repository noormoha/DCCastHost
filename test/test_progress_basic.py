import unittest
import time


import os
script_path = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), "scripts")
import sys
sys.path.insert(0, script_path)
import command

from test_config import test_topo, port


class TestProgress(unittest.TestCase):

    def setUp(self):
        err = command.receive(test_topo[0][3], port, 0, 2000)
        self.assertIsNone(err)
        err = command.send(test_topo[0][2], port, 0, test_topo[0][3], 2000, 1000000)
        self.assertIsNone(err)

    def test_progress(self):
        time.sleep(20)
        # 20s * 1000000bit/s / 8bit/byte = 2500000 bytes
        err, res = command.get_progress(test_topo[0][3], port, 0)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 2500000 * 0.8)
        self.assertLess(res['progress'], 2500000 / 0.8)
        err, res = command.get_progress(test_topo[0][2], port, 0)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 2500000 * 0.8)
        self.assertLess(res['progress'], 2500000 / 0.8)
        
    def tearDown(self):
        err = command.clean_all(test_topo[0][2], port)
        self.assertIsNone(err)
        err = command.clean_all(test_topo[0][3], port)
        self.assertIsNone(err)


if __name__ == '__main__':
    unittest.main()
