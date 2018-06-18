""" Test Bed for DCCast Host
"""

import geni.portal as portal
import geni.rspec.pg as rspec

base_capacity = 1000000
num_nodes = 3

test_topo = [
    [0, 1, "192.168.100.1", "192.168.100.3"],
    [1, 2, "192.168.100.4", "192.168.100.5"],
    [0, 2, "192.168.100.2", "192.168.100.6"]
]

request = portal.context.makeRequestRSpec()

nodes = []
interfaces = []

for i in range(num_nodes):
    nodes.append(request.XenVM("node%d" % i))
    nodes[i].disk_image = "urn:publicid:IDN+emulab.net+image+emulab-ops//UBUNTU16-64-STD"

    nodes[i].addService(rspec.Execute(shell="sh", command="wget -O /tmp/endpoint.sh https://raw.githubusercontent.com/noormoha/cloudlab-wancast-config/master/endpoint.sh"))
    nodes[i].addService(rspec.Execute(shell="sh", command="sh /tmp/endpoint.sh %d > /tmp/endpoint.log" % (i + 1)))


lan_id = 0
for topo_link in test_topo:
    link = request.LAN("lan%d" % lan_id)
    lan_id += 1
    link.bandwidth = base_capacity

    iface1 = nodes[topo_link[0]].addInterface("ifto%d" % topo_link[1])
    iface1.addAddress(rspec.IPv4Address(topo_link[2], "255.255.255.0"))
    iface2 = nodes[topo_link[1]].addInterface("ifto%d" % topo_link[0])
    iface2.addAddress(rspec.IPv4Address(topo_link[3], "255.255.255.0"))

    link.addInterface(iface1)
    link.addInterface(iface2)

portal.context.printRequestRSpec()

