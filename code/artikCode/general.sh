sysctl net.ipv4.ip_forward=1
iptables --flush
iptables -t nat --flush
iptables --delete-chain
iptables -t nat --delete-chain
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
iptables -A FORWARD -i wlan0 -j ACCEPT
