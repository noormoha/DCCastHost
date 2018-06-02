"""
Hi Alston,

Now I am facing an even weirder error! Could you login to node5 from WC130?

When I call curl http://192.168.1.7:9080/progress/0 on node5 it shows that the sender is sending but when I run on node5

sudo tcpdump -i any port 1000 -c 5

It captures no packets!

Could the sender think it is sending but actually it is not???

I would appreciate it if you looked at this.

Best,
Max

REST POST -> http://192.168.1.2:9080/cleanAll, Body:
{"error":null}
REST POST -> http://192.168.1.3:9080/cleanAll, Body:
{"error":null}
REST POST -> http://192.168.1.4:9080/cleanAll, Body:
{"error":null}
REST POST -> http://192.168.1.5:9080/cleanAll, Body:
{"error":null}
REST POST -> http://192.168.1.6:9080/cleanAll, Body:
{"error":null}
REST POST -> http://192.168.1.7:9080/cleanAll, Body:
{"error":null}
REST POST -> http://192.168.1.8:9080/cleanAll, Body:
{"error":null}
REST POST -> http://192.168.1.9:9080/cleanAll, Body:
{"error":null}
REST POST -> http://192.168.1.10:9080/cleanAll, Body:
{"error":null}
REST POST -> http://192.168.1.11:9080/cleanAll, Body:
{"error":null}
REST POST -> http://192.168.1.12:9080/cleanAll, Body:
{"error":null}
REST POST -> http://192.168.1.13:9080/cleanAll, Body:
{"error":null}

REST POST -> http://192.168.1.13:9080/receive/0?src=10.10.10.6&port=1000, Body:
{"debug":{"id":0,"port":1000,"src":"10.10.10.6"},"error":null}
REST POST -> http://192.168.1.7:9080/send/0?dst=10.10.10.12&port=1000&rate=16666666, Body:
{"debug":{"dst":"10.10.10.12","id":0,"port":1000,"rate":16666666.0},"error":null}
REST POST -> http://192.168.1.10:9080/receive/1?src=10.10.10.3&port=1001, Body:
{"debug":{"id":1,"port":1001,"src":"10.10.10.3"},"error":null}
REST POST -> http://192.168.1.4:9080/send/1?dst=10.10.10.9&port=1001&rate=16666666, Body:
{"debug":{"dst":"10.10.10.9","id":1,"port":1001,"rate":16666666.0},"error":null}
REST POST -> http://192.168.1.13:9080/receive/2?src=10.10.10.6&port=1002, Body:
{"debug":{"id":2,"port":1002,"src":"10.10.10.6"},"error":null}
REST POST -> http://192.168.1.7:9080/send/2?dst=10.10.10.12&port=1002&rate=16666666, Body:
{"debug":{"dst":"10.10.10.12","id":2,"port":1002,"rate":16666666.0},"error":null}

REST GET -> http://192.168.1.7:9080/progress/0
Req 0: bits sent by sender 5: 14991360
REST GET -> http://192.168.1.13:9080/progress/0
Req 0: bits received by receiver 11: 0
"""


import unittest
import time


import os
script_path = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), "scripts")
import sys
sys.path.insert(0, script_path)
import command

from test_config import test_topo, port


class TestIssue(unittest.TestCase):

    def setUp(self):
        for i in range(2, 14):
            err = command.clean_all("192.168.1.{}".format(i), port)
            self.assertIsNone(err)
        time.sleep(2)

    def test_issue(self):
        err = command.receive("192.168.1.13", port, 0, 1000)
        self.assertIsNone(err)
        err = command.send("192.168.1.7", port, 0, "10.10.10.12", 1000, 16666666)
        self.assertIsNone(err)
        err = command.receive("192.168.1.10", port, 1, 1001)
        self.assertIsNone(err)
        err = command.send("192.168.1.4", port, 1, "10.10.10.9", 1001, 16666666)
        self.assertIsNone(err)
        err = command.receive("192.168.1.13", port, 2, 1002)
        self.assertIsNone(err)
        err = command.send("192.168.1.7", port, 2, "10.10.10.12", 1002, 16666666)
        self.assertIsNone(err)

        time.sleep(10)
        err, res = command.get_progress("192.168.1.7", port, 0)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)
        err, res = command.get_progress("192.168.1.7", port, 2)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)
        err, res = command.get_progress("192.168.1.4", port, 1)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)