import command
import time

nodes = ["10.10.10.1", "10.10.10.2"]

num_sender = 10
bandwidth = 200000000

for i in range(num_sender):
    err = command.receive(nodes[1], 9080, i, 1230 + i)
    if err:
        print(err)
        break
    err = command.send(nodes[0], 9080, i, nodes[1], 1230 + i, bandwidth // num_sender)
    if err:
        print(err)
        break
