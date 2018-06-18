"""
timeslot 3
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

timeslot4
REST GET -> http://192.168.1.7:9080/progress/0
Req 0: bits sent by sender 5: 14827520
REST GET -> http://192.168.1.13:9080/progress/0
Req 0: bits received by receiver 11: 10649600
Progress of transfer 0 to this point: 5.0%
REST GET -> http://192.168.1.4:9080/progress/1
Req 1: bits sent by sender 2: 13844480
REST GET -> http://192.168.1.10:9080/progress/1
Req 1: bits received by receiver 8: 13455680
Progress of transfer 1 to this point: 6.0%
REST GET -> http://192.168.1.7:9080/progress/2
Req 2: bits sent by sender 5: 12861440
REST GET -> http://192.168.1.13:9080/progress/2
Req 2: bits received by receiver 11: 9533920
Progress of transfer 2 to this point: 2.0%
REST POST -> http://192.168.1.7:9080/updateRate/0?rate=16666666, Body:
{"debug":{"id":0,"rate":16666666.0},"err":null}
REST POST -> http://192.168.1.4:9080/updateRate/1?rate=16666666, Body:
{"debug":{"id":1,"rate":16666666.0},"err":null}
REST POST -> http://192.168.1.7:9080/updateRate/2?rate=16666666, Body:
{"debug":{"id":2,"rate":16666666.0},"err":null}
REST POST -> http://192.168.1.11:9080/receive/3?src=10.10.10.7&port=1003, Body:
{"debug":{"id":3,"port":1003,"src":"10.10.10.7"},"error":null}
REST POST -> http://192.168.1.8:9080/send/3?dst=10.10.10.10&port=1003&rate=20000000, Body:
{"debug":{"dst":"10.10.10.10","id":3,"port":1003,"rate":20000000.0},"error":null}
"""


import unittest
import time

from log_parser import parse


import os
script_path = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), "scripts")
import sys
sys.path.insert(0, script_path)
import command

from test_config import test_topo, port

time_unit = 1


def test_issue():
    commands = parse("issue2.log")
    id_ip_map = {}
    cur_time = 0
    for command_time in commands:
        if command_time > cur_time:
            time.sleep(command_time - cur_time)
            cur_time = command_time

        for command_item in commands[command_time]:
            print(command_item)
            if command_item["type"] == "cleanAll":
                err = command.clean_all(command_item["host"], port)
                if err:
                    print(cur_time)
                    print(command_item)
                    print(err)
            elif command_item["type"] == "progress":
                err = command.get_progress(command_item["host"], port, command_item["id"])
                if err:
                    print(cur_time)
                    print(command_item)
                    print(err)
            elif command_item["type"] == "updateRate":
                err = command.update_rate(command_item["host"], port, command_item["id"], command_item["rate"])
                if err:
                    print(cur_time)
                    print(command_item)
                    print(err)
            elif command_item["type"] == "receive":
                err = command.receive(command_item["host"], port, command_item["id"], command_item["port"])
                if err:
                    print(cur_time)
                    print(command_item)
                    print(err)
                id_ip_map[command_item["id"]] = command_item["host"]
            elif command_item["type"] == "send":
                if id_ip_map.get(command_item["id"], None) is None:
                    print("id_ip_map is none")
                err = command.send(command_item["host"], port, command_item["id"], id_ip_map[command_item["id"]], command_item["port"], command_item["rate"])
                if err:
                    print(cur_time)
                    print(command_item)
                    print(err)




"""
        time.sleep(3 * time_unit)
        # At time slot 3

        err = command.receive("192.168.1.13", port, 0, 1000)
        self.assertIsNone(err)
        err = command.send("192.168.1.7", port, 0, "192.168.1.13", 1000, 16666666)
        self.assertIsNone(err)

        err = command.receive("192.168.1.10", port, 1, 1001)
        self.assertIsNone(err)
        err = command.send("192.168.1.4", port, 1, "192.168.1.10", 1001, 16666666)
        self.assertIsNone(err)

        err = command.receive("192.168.1.13", port, 2, 1002)
        self.assertIsNone(err)
        err = command.send("192.168.1.7", port, 2, "192.168.1.13", 1002, 16666666)
        self.assertIsNone(err)

        time.sleep(1 * time_unit)
        # At time slot 4

        err, res = command.get_progress("192.168.1.13", port, 0)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)
        err, res = command.get_progress("192.168.1.7", port, 0)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)

        err, res = command.get_progress("192.168.1.10", port, 1)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)
        err, res = command.get_progress("192.168.1.4", port, 1)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)

        err, res = command.get_progress("192.168.1.13", port, 2)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)
        err, res = command.get_progress("192.168.1.7", port, 2)
        self.assertIsNone(err)
        self.assertGreater(res['progress'], 0)

        err = command.update_rate("192.168.1.7", port, 0, 16666666)
        self.assertIsNone(err)
        err = command.update_rate("192.168.1.10", port, 1, 16666666)
        self.assertIsNone(err)
        err = command.update_rate("192.168.1.7", port, 2, 16666666)
        self.assertIsNone(err)

        err = command.receive("192.168.1.11", port, 3, 1003)
        self.assertIsNone(err)
        err = command.send("192.168.1.8", port, 3, "192.168.1.11", 1003, 20000000)
        self.assertIsNone(err)
"""

if __name__ == "__main__":
    test_issue()
