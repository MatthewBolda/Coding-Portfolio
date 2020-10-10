# !/bin/sh

#Remove any previous rules or chains
sudo iptables -F
sudo iptables -t filter -F
sudo iptables -t filter -X
sudo iptables -t mangle -F
sudo iptables -t mangle -X
sudo iptables -t nat -F
sudo iptables -t nat -X
sudo iptables -t raw -F
sudo iptables -t raw -X

#change source IP address to your own machine's IP address
sudo iptables -t nat -A POSTROUTING -o enp0s3 -j MASQUERADE

#block a list of specific IP addresses
#nsilookup for google.com and then 2 randoms
sudo iptables -A INPUT -s 255.255.255.255 -j DROP
sudo iptables -A INPUT -s 10.10.10.10 -j DROP
sudo iptables -A INPUT -s 172.217.6.110 -j DROP

#block your computer from being pinged by all other hosts
sudo iptables -A INPUT -p icmp --icmp-type echo-request -j DROP

#set up port-forwarding from an unused port of your port 22
sudo iptables -A PREROUTING -t nat -i enp0s3 -p tcp --dport 128 -j DNAT --to 192.168.1.127:22
sudo iptables -A FORWARD -p tcp -d 192.168.1.127 --dport 22 -j ACCEPT

#allow for SSH access (port 22) to your machine from engineering.purdue.edu domain
sudo iptables -A INPUT -p tcp -s engineering.purdue.edu --dport 22 -j ACCEPT
sudo iptables -A INPUT -p tcp --dport 22 -j REJECT

#write a rule that allows for a single IP address to access machine
sudo iptables -A INPUT -p tcp -s 192.168.1.125 --dport 80 -j ACCEPT
sudo iptables -A INPUT -p tcp -j REJECT

#Permit Auth/Ident (port 113) that is used by some services
sudo iptables -A INPUT -i enp0s3 -p tcp --dport 113 --syn -j ACCEPT
