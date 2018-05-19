import command
import time

nodes = ["10.0.0.1", "10.0.0.2"]

cur = 500000000
min_speed = 1000000000

while True:
    start_time = time.time()

    err = command.receive(nodes[1], 9080, 0, 1234)
    if err:
        print(err)
        break
    err = command.send(nodes[0], 9080, 0, nodes[1], 1234, cur)
    if err:
        print(err)
        break

    time.sleep(30)

    err = command.terminate(nodes[0], 9080, 0)
    if err:
        print(err)
        break
    err = command.terminate(nodes[1], 9080, 0)
    if err:
        print(err)
        break

    end_time = time.time()

    time.sleep(5)

    err, res_sender = command.get_progress(nodes[0], 9080, 0)
    if err:
        print(err)
        break

    err, res_receiver = command.get_progress(nodes[1], 9080, 0)
    if err:
        print(err)
        break

    expected = cur * (end_time - start_time)

    print ("Expect {} bits".format(expected))
    print("Sender sends {} bits".format(res_sender["progress"] * 8))
    print("Receiver receives {} bits".format(res_receiver["progress"] * 8))
    cur = min(res_sender["progress"] * 8, res_receiver["progress"] * 8)
    min_speed = min(min_speed, cur)

    print("Current min speed {}".format(min_speed))
