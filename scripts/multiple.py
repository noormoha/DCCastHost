import command
import time

nodes = ["10.0.0.1", "10.0.0.2"]

num_sender = 10
bandwidth = 100000000

start_time = time.time()

for i in range(num_sender):
    err = command.receive(nodes[1], 9080, i, 1230 + i)
    if err:
        print(err)
        break

time.sleep(1)

for i in range(num_sender):
    err = command.send(nodes[0], 9080, i, nodes[1], 1230 + i, bandwidth // num_sender)
    if err:
        print(err)
        break

time.sleep(30)

end_time = time.time()

total_count = 0

for i in range(num_sender):
    err = command.terminate(nodes[0], 9080, i)
    if err:
        print(err)
    err = command.terminate(nodes[1], 9080, i)
    if err:
        print(err)

    err, receive_res = command.get_progress(nodes[1], 9080, i)
    if err:
        print(err)
    print("Node {} transfer {} receives {} bytes | expected speed: {} | actual speed: {}"
          .format(1, i, receive_res["progress"], bandwidth // num_sender, receive_res["progress"] * 8 // (end_time - start_time)))
    total_count += receive_res["progress"]

    err, send_res = command.get_progress(nodes[0], 9080, i)
    if err:
        print(err)
    print("Node {} transfer {} send {} bytes | expected speed: {} | actual speed: {}"
          .format(0, i, send_res["progress"], bandwidth // num_sender, send_res["progress"] * 8 // (end_time - start_time)))

actual_bandwidth = total_count * 8 // (end_time - start_time)
print("Actual total speed: {} | expected speed {} | {}%". format(actual_bandwidth, bandwidth, actual_bandwidth / bandwidth * 100))
