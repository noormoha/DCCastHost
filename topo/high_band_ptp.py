from mininet.topo import Topo
from mininet.net import Mininet
from mininet.cli import CLI
from mininet.link import TCLink
from mininet.util import dumpNodeConnections


class HostToHostTopo( Topo ):
    def build(self):
        h1 = self.addHost('h1')
        h2 = self.addHost('h2')
        self.addLink(h1, h2, bw=1000)


if __name__ == '__main__':
    topo = HostToHostTopo()
    net = Mininet(topo=topo, link=TCLink)
    net.start()
    print "Dumping host connections"
    dumpNodeConnections(net.hosts)
    print "Testing network connectivity"
    net.pingAll()
    print "Testing bandwidth"
    h1, h2 = net.get('h1', 'h2')
    net.iperf((h1, h2))
    CLI(net)