import unittest
import os
script_path = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), "scripts")
import sys
sys.path.insert(0, script_path)
import command

from test_config import test_topo, port
import time


class TestSilentFailure(unittest.TestCase):

    def setUp(self):
        err = command.receive(test_topo[0][3], port, 0, 1000)
        self.assertIsNone(err)
        err = command.send(test_topo[0][2], port, 0, test_topo[0][3], 1000, 1000000)
        self.assertIsNone(err)
        time.sleep(10)

    def test_port_reuse(self):
        # Use a different transfer ID and the same port number
        # DCCastHost will try to create NORM instance but fail at NORMStartReceiver
        err = command.receive(test_topo[0][3], port, 1, 1000)
        self.assertEqual(err, "Timeout when waiting for response")
        err = command.send(test_topo[0][2], port, 1, test_topo[0][3], 1000, 1000000)
        self.assertEqual(err, "Timeout when waiting for response")

        # If we use a different port, we should see transmission
        err = command.receive(test_topo[0][3], port, 1, 1001)
        self.assertEqual(err, None)
        err = command.send(test_topo[0][2], port, 1, test_topo[0][3], 1001, 1000000)
        self.assertEqual(err, None)

        time.sleep(5)

        err, res = command.get_progress(test_topo[0][3], port, 1)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)
        err, res = command.get_progress(test_topo[0][2], port, 1)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)

    def tearDown(self):
        err = command.clean_all(test_topo[0][2], port)
        self.assertIsNone(err)
        err = command.clean_all(test_topo[0][3], port)
        self.assertIsNone(err)


class TestSendFollowCleanAll(unittest.TestCase):
    def setUp(self):
        err = command.receive(test_topo[0][3], port, 0, 1000)
        self.assertIsNone(err)
        err = command.send(test_topo[0][2], port, 0, test_topo[0][3], 1000, 1000000)
        self.assertIsNone(err)
        time.sleep(5)

    def test_clean_restart(self):
        err = command.clean_all(test_topo[0][2], port)
        self.assertIsNone(err)
        err = command.clean_all(test_topo[0][3], port)
        self.assertIsNone(err)

        err = command.receive(test_topo[0][3], port, 0, 1000)
        self.assertIsNone(err)
        err = command.send(test_topo[0][2], port, 0, test_topo[0][3], 1000, 1000000)
        self.assertIsNone(err)
        time.sleep(5)
        err, res = command.get_progress(test_topo[0][3], port, 0)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)
        err, res = command.get_progress(test_topo[0][2], port, 0)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)

    def tearDown(self):
        err = command.clean_all(test_topo[0][2], port)
        self.assertIsNone(err)
        err = command.clean_all(test_topo[0][3], port)
        self.assertIsNone(err)

if __name__ == "__main__":
    unittest.main()