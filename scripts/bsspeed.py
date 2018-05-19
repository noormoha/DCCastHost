import command
import time

nodes = ["10.0.0.1", "10.0.0.2"]


l, r = 1000000, 1000000000

while l < r:
    m = (l + r) // 2
    print("Test at rate {}".format(m))

    start_time = time.time()

    err = command.receive(nodes[1], 9080, 0, 1234)
    if err:
        print(err)
        break
    err = command.send(nodes[0], 9080, 0, nodes[1], 1234, m)
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

    expected = m * (end_time - start_time)

    success = True
    print ("Expect {} bits".format(expected))
    print("Sender sends {} bits".format(res_sender["progress"] * 8))
    print("Receiver receives {} bits".format(res_receiver["progress"] * 8))
    if abs(res_sender["progress"] * 8 - expected) / expected > 0.2:
        success = False
    if abs(res_receiver["progress"] * 8 - expected) / expected > 0.2:
        success = False

    if success:
        l = m + 1
    else:
        r = m - 1

