import unittest
import time


import os
script_path = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), "scripts")
import sys
sys.path.insert(0, script_path)
import command

from test_config import test_topo, port


class Test200MSingle(unittest.TestCase):

    def setUp(self):
        err = command.receive(test_topo[0][3], port, 0, 2000)
        self.assertIsNone(err)
        err = command.send(test_topo[0][2], port, 0, test_topo[0][3], 2000, 200000000)
        self.assertIsNone(err)

    def test_speed(self):
        time.sleep(20)
        # 20s * 200 * 10^6bit/s / 8bit/byte = 500 * 10^6 bytes
        err, res = command.get_progress(test_topo[0][3], port, 0)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 500000000 * 0.8)
        self.assertLess(res['progress'], 500000000 / 0.8)
        err, res = command.get_progress(test_topo[0][2], port, 0)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 500000000 * 0.8)
        self.assertLess(res['progress'], 500000000 / 0.8)

    def tearDown(self):
        err = command.clean_all(test_topo[0][2], port)
        self.assertIsNone(err)
        err = command.clean_all(test_topo[0][3], port)
        self.assertIsNone(err)


class Test200Multiple(unittest.TestCase):
    start_time = []

    def setUp(self):
        for i in range(0, 20):
            self.start_time.append(time.time())
            err = command.receive(test_topo[0][3], port, i, 2000 + i)
            self.assertIsNone(err)
            err = command.send(test_topo[0][2], port, i, test_topo[0][3], 2000 + i, 200000000 / 20)
            self.assertIsNone(err)

    def test_speed(self):
        time.sleep(20)
        # 20s * 200 * 10^6bit/s / 8bit/byte = 500 * 10^6 bytes
        for i in range(0, 20):
            elapse = time.time() - self.start_time[i]
            err, res = command.get_progress(test_topo[0][3], port, i)
            self.assertIsNone(err)
            self.assertGreater(res['progress'], 200000000 / 20 * elapse / 8 * 0.8)
            self.assertLess(res['progress'], 200000000 / 20 * elapse / 8 / 0.8)
            err, res = command.get_progress(test_topo[0][2], port, i)
            self.assertIsNone(err)
            self.assertGreater(res['progress'], 200000000 / 20 * elapse / 8 * 0.8)
            self.assertLess(res['progress'], 200000000 / 20 * elapse / 8 / 0.8)

    def tearDown(self):
        err = command.clean_all(test_topo[0][2], port)
        self.assertIsNone(err)
        err = command.clean_all(test_topo[0][3], port)
        self.assertIsNone(err)


if __name__ == '__main__':
    unittest.main()
