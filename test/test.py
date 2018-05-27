import unittest
import time


import os
script_path = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), "scripts")
import sys
sys.path.insert(0, script_path)
import command

test_topo = [
    [0, 1, "10.0.0.1", "10.0.0.2"]
]

port = 9080

class TestBasic(unittest.TestCase):

    def setUp(self):
        err = command.receive(test_topo[0][3], port, 0, 2000)
        self.assertIsNone(err)
        err = command.send(test_topo[0][2], port, 0, test_topo[0][3], 2000, 1000000)
        self.assertIsNone(err)
        time.sleep(10)

    def test_progress(self):
        err, res = command.get_progress(test_topo[0][3], port, 0)
        print(res)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)
        err, res = command.get_progress(test_topo[0][2], port, 0)
        print(res)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)

    def tearDown(self):
        err = command.clean_all(test_topo[0][2], port)
        self.assertIsNone(err)
        err = command.clean_all(test_topo[0][3], port)
        self.assertIsNone(err)


class TestLargeTransferID(unittest.TestCase):

    def setUp(self):
        err = command.receive(test_topo[0][3], port, 100000, 2001)
        self.assertIsNone(err)
        err = command.send(test_topo[0][2], port, 100000, test_topo[0][3], 2001, 1000000)
        self.assertIsNone(err)
        time.sleep(10)

    def test_progress(self):
        err, res = command.get_progress(test_topo[0][3], port, 100000)
        print(res)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)
        err, res = command.get_progress(test_topo[0][2], port, 100000)
        print(res)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)

    def tearDown(self):
        err = command.clean_all(test_topo[0][2], port)
        self.assertIsNone(err)
        err = command.clean_all(test_topo[0][3], port)
        self.assertIsNone(err)


class TestCleanAll(unittest.TestCase):

    def setUp(self):
        for i in range(3):
            err = command.receive(test_topo[0][3], port, 15 + i, 2010 + i)
            self.assertIsNone(err)
            err = command.send(test_topo[0][2], port, 15 + i, test_topo[0][3], 2010 + i, 1000000)
            self.assertIsNone(err)
        time.sleep(10)

    def test_active_transfer(self):
        err, res_json = command.active_transfer(test_topo[0][2], port)
        self.assertIsNone(err)
        self.assertEqual(len(res_json['senders']), 3)
        err, res_json = command.active_transfer(test_topo[0][3], port)
        self.assertIsNone(err)
        self.assertEqual(len(res_json['receivers']), 3)

        err = command.clean_all(test_topo[0][2], port)
        self.assertIsNone(err)
        err = command.clean_all(test_topo[0][3], port)
        self.assertIsNone(err)

        time.sleep(2)

        err, res_json = command.active_transfer(test_topo[0][2], port)
        self.assertIsNone(err)
        self.assertEqual(len(res_json['senders']), 0)
        err, res_json = command.active_transfer(test_topo[0][3], port)
        self.assertIsNone(err)
        self.assertEqual(len(res_json['receivers']), 0)





if __name__ == '__main__':
    unittest.main()
