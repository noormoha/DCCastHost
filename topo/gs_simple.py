from mininet.topo import Topo
from mininet.net import Mininet
from mininet.cli import CLI
from mininet.util import dumpNodeConnections


class HostToHostTopo( Topo ):
    switch = None
    def build(self):
        self.switch = self.addSwitch('s1')
        for i in range(2, 14):
            host = self.addHost("h{}".format(i), ip="192.168.1.{}/24".format(i))
            self.addLink(self.switch, host)


if __name__ == '__main__':
    topo = HostToHostTopo()
    net = Mininet(topo=topo)
    net.start()
    print "Dumping host connections"
    dumpNodeConnections(net.hosts)
    print "Testing network connectivity"
    net.pingAll()
    CLI(net)