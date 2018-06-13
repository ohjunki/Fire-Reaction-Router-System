sysctl net.ipv4.ip_forward=1
iptables --flush
iptables -t nat --flush
iptables --delete-chain
iptables -t nat --delete-chain

iptables -t nat -A PREROUTING -p tcp --dport 80 -j DNAT --to-destination 192.168.1.1:3000
iptables -t nat -A PREROUTING -p udp --dport 80 -j DNAT --to-destination 192.168.1.1:3000
iptables -A FORWARD -i wlan0 -j ACCEPT
