Networking is the bloodstream of CTFs. Every remote exploit, every command‑and‑control channel, every packet injection attack relies on understanding how machines talk to each other. This week you’ll stop treating the network as a black box and start crafting raw packets, building servers, and intercepting traffic like an attacker. By the end, you’ll have written a TCP stack client, a port scanner, and even spoofed ARP packets—the foundation of man‑in‑the‑middle attacks.

# 🔥 WEEK 11: SUPER INTENSIVE DEEP DIVE  
## *“Packets & Protocols”*

---

## ⏳ Session Overview (8‑Hour Immersive Block)

| Time (total) | Section |
|--------------|---------|
| 0:00 – 1:30 | The TCP/IP Stack & Packet Anatomy |
| 1:30 – 3:30 | Sockets in C: TCP Client/Server |
| 3:30 – 4:30 | Raw Sockets & Packet Crafting |
| 4:30 – 5:00 | Netcat, Nmap & Network Recon |
| 5:00 – 6:00 | Scapy: Python Packet Manipulation |
| 6:00 – 7:00 | Network Security Concepts (Sniffing, Spoofing, MITM) |
| 7:00 – 8:00 | Mini‑Project: Port Scanner & Problem Set Kickoff |

---

## 🧱 PART 1: THE TCP/IP STACK & PACKET ANATOMY (1.5 HOURS)

Before we code a single socket, you must understand the layered architecture that makes the internet possible.

### 1.1 The Four Layers
1. **Link Layer** (Ethernet, ARP, MAC addresses)  
2. **Internet Layer** (IP – addressing and routing)  
3. **Transport Layer** (TCP, UDP – ports, reliability)  
4. **Application Layer** (HTTP, DNS, FTP, etc.)

Each layer encapsulates the layer above it. Data moves down the stack on the sender and up on the receiver.

### 1.2 Ethernet Frame (Link Layer)
An Ethernet frame contains:
- **Destination MAC** (6 bytes)
- **Source MAC** (6 bytes)
- **EtherType** (2 bytes, e.g., 0x0800 for IPv4)
- **Payload** (IP packet)
- **CRC** (4 bytes, hardware‑generated)

**CTF relevance:** ARP spoofing works by sending false ARP replies, poisoning the MAC‑IP mapping. You need to understand Ethernet frames to craft them.

### 1.3 IP Header (Internet Layer)
An IPv4 header is typically 20 bytes, containing:
- Version (4), IHL, Type of Service, Total Length
- Identification, Flags, Fragment Offset
- TTL, Protocol (6=TCP, 17=UDP), Header Checksum
- **Source IP** and **Destination IP**

**Key fields for attacks:** Source IP spoofing (for blind attacks or reflection), TTL manipulation, fragmentation attacks.

### 1.4 TCP Header (Transport Layer)
- **Source Port, Destination Port** (16‑bit each)
- **Sequence Number, Acknowledgment Number**
- **Data Offset, Flags** (SYN, ACK, FIN, RST, etc.), Window Size
- Checksum, Urgent Pointer

**The famous three‑way handshake:**
1. Client → Server: `SYN` (seq = X)
2. Server → Client: `SYN+ACK` (seq = Y, ack = X+1)
3. Client → Server: `ACK` (seq = X+1, ack = Y+1)

**CTF relevance:** SYN flood attacks, TCP session hijacking (guessing sequence numbers), RST injection (breaking connections), and covert channels.

### 1.5 UDP and ICMP (Briefly)
- **UDP:** connectionless, no handshake, easy to spoof.
- **ICMP:** control messages (ping, destination unreachable), used in tunneling and exfiltration.

### 1.6 Wireshark Dissection (Hands‑On)
Open Wireshark and capture a simple `curl http://example.com`. Walk through the captured packets:
- Find the DNS query/response.
- Locate the TCP three‑way handshake.
- Follow the HTTP stream.
- Inspect the raw bytes of headers.

**Exercise:** Capture your own traffic while performing a netcat connection. Identify each layer's headers.

---

## 🖥️ PART 2: SOCKETS IN C – TCP CLIENT/SERVER (2 HOURS)

You’ve used Python sockets. Now we do it in C, where you control every byte.

### 2.1 Socket Creation (`socket()`)
```c
#include <sys/socket.h>
#include <netinet/in.h>
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
```
`AF_INET` = IPv4, `SOCK_STREAM` = TCP.

### 2.2 TCP Client
Steps:
1. `socket()` to create.
2. `connect()` to server (IP, port).
3. `send()` / `recv()` to communicate.
4. `close()`.

```c
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons(80);               // port in network byte order
inet_pton(AF_INET, "93.184.216.34", &addr.sin_addr); // IP string to binary

if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("connect"); exit(1);
}
char *request = "GET / HTTP/1.0\r\nHost: example.com\r\n\r\n";
send(sockfd, request, strlen(request), 0);
char buf[4096];
int n = recv(sockfd, buf, sizeof(buf)-1, 0);
buf[n] = '\0';
printf("%s\n", buf);
```
**Code‑along:** Write a client that downloads a webpage from any server. Change the port and experiment.

### 2.3 TCP Server
Steps:
1. `socket()`
2. `bind()` to a local address and port.
3. `listen()` for incoming connections.
4. `accept()` a client, creating a new socket for communication.
5. `send()`/`recv()`.

```c
int servfd = socket(AF_INET, SOCK_STREAM, 0);
struct sockaddr_in serv_addr;
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = INADDR_ANY;   // listen on all interfaces
serv_addr.sin_port = htons(9999);

bind(servfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
listen(servfd, 5);
printf("Listening on port 9999...\n");

struct sockaddr_in client_addr;
socklen_t client_len = sizeof(client_addr);
int clientfd = accept(servfd, (struct sockaddr*)&client_addr, &client_len);

char buffer[1024];
int n = recv(clientfd, buffer, sizeof(buffer)-1, 0);
buffer[n] = '\0';
printf("Received: %s\n", buffer);
send(clientfd, "Hello from server\n", 18, 0);
close(clientfd);
close(servfd);
```

**Exercise:** Build an echo server that sends back everything the client sends until the client disconnects. Use a loop with `recv` until 0 bytes received.

### 2.4 A Tiny HTTP Server
Extend the echo server to parse the first line of the request (`GET /path HTTP/1.0`) and respond with a hard‑coded HTML page. This forces you to understand the HTTP protocol at the byte level, which is essential for web challenges.

**CTF connection:** Many CTFs present a custom protocol. Writing C servers trains you to read raw specifications and implement them, which is exactly what you do to communicate with a remote flag server.

---

## 🔧 PART 3: RAW SOCKETS & PACKET CRAFTING (1 HOUR)

Raw sockets give you access to the IP layer and below, allowing you to craft arbitrary packets for spoofing and injection attacks.

### 3.1 Creating a Raw Socket
```c
int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP); // or IPPROTO_RAW
```
Requires root privileges (`sudo`). This allows you to set your own IP and TCP headers.

### 3.2 Crafting a SYN Packet
We’ll construct a TCP SYN packet to a target, with a spoofed source IP (for demonstration purposes only on a local, isolated network or with explicit permission).

- Build an IP header (struct iphdr) and TCP header (struct tcphdr).
- Use `sendto()` on the raw socket.
- Learn about checksum calculation: required for IP and TCP headers (we can use a helper function).

**Important:** Linux requires the kernel to not overwrite headers; set socket option `IP_HDRINCL`.

**Demo (simplified):** Use `nping` or `scapy` first to understand, then attempt a raw socket in C for educational purposes. I’ll provide a safe skeleton.

```c
int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
if (sock < 0) { perror("raw socket"); exit(1); }
int on = 1;
setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));

// Build payload manually (IP + TCP headers)
char packet[4096];
struct iphdr *iph = (struct iphdr*)packet;
struct tcphdr *tcph = (struct tcphdr*)(packet + sizeof(struct iphdr));
// fill iph and tcph fields...
// sendto(sock, packet, total_len, ...);
```
We won’t go deep into header structs here, but the knowledge that it’s possible is crucial for later exploit development (e.g., crafting a specific packet to bypass firewall rules).

### 3.3 Packet Sniffing with Raw Sockets
You can also receive all packets by setting the socket to promiscuous mode. This is how tools like `tcpdump` start.

```c
int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
// recvfrom into a buffer, parse Ethernet/IP/TCP headers
```

**Exercise:** Write a simple sniffer that counts how many TCP, UDP, and ICMP packets arrive on `eth0` over 10 seconds. This introduces you to low‑level monitoring.

**CTF connection:** Network forensics challenges often provide a pcap file; understanding how packets are structured helps write custom filters and extract hidden data.

---

## 📡 PART 4: NETCAT, NMAP & NETWORK RECON (0.5 HOUR)

### 4.1 Netcat – The TCP Swiss Army Knife
- `nc -l -p 1234` — listen.
- `nc host 80` — connect and interact.
- `nc -e /bin/bash` — bind a shell (used in some CTFs to catch flags).
- `nc -v host 1-1000` — port scan (though nmap is better).

**Exercise:** Use netcat to transfer a file between two terminals.

### 4.2 Nmap – The Network Mapper
```bash
nmap -sV 192.168.1.1  # service version detection
nmap -sS 192.168.1.0/24  # SYN scan (stealth)
nmap -O target      # OS fingerprinting
```

**CTF connection:** In many attack‑defense CTFs, you need to enumerate services on opponents’ machines quickly. Nmap scripting engine (NSE) can automate many probes.

---

## 🐍 PART 5: SCAPY – PYTHON PACKET MANIPULATION (1 HOUR)

Scapy is the ultimate tool for packet crafting, sniffing, and protocol analysis—all in Python. It’s a CTF staple.

### 5.1 Basics: Creating and Sending a Packet
```python
from scapy.all import *
# Craft a SYN packet
ip = IP(dst="192.168.1.1")
tcp = TCP(dport=80, flags="S")
pkt = ip/tcp
# Send and receive response
ans = sr1(pkt, timeout=2)
ans.show()
```

### 5.2 ARP Spoofing Demo
We can poison a target’s ARP table by sending unsolicited ARP replies:
```python
arp = ARP(op=2, pdst="192.168.1.10", psrc="192.168.1.1", hwdst="ff:ff:ff:ff:ff:ff")
while True:
    send(arp, verbose=0)
```
(Only do this on your own isolated VM network.)

**Exercise:** Build a simple MiTM script that poisons ARP and then forwards captured packets using your Python knowledge. Not today, but later.

### 5.3 Sniffing with Scapy
```python
def packet_handler(pkt):
    if IP in pkt:
        print(pkt[IP].src, "->", pkt[IP].dst, ":", pkt[IP].len)
sniff(iface="eth0", prn=packet_handler, count=10)
```
Scapy can also dissect entire protocols with `pkt.show()`.

**CTF connection:** In many CTFs, you’ll get a pcap and must extract a flag hidden in a custom protocol. Scapy can parse it, or you can build a custom dissector.

---

## 🛡️ PART 6: NETWORK SECURITY CONCEPTS (1 HOUR)

### 6.1 Sniffing
- **Passive:** listen on a shared medium (hub, or mirrored port).
- **Active:** ARP spoofing to force traffic through your machine.

### 6.2 Spoofing
- **IP Spoofing:** faking source IP (blind). Used in reflection attacks.
- **ARP Spoofing:** pretending to be the gateway.
- **DNS Spoofing:** responding with fake DNS answers.

### 6.3 Man‑in‑the‑Middle (MitM)
Intercept and modify traffic in transit. Tools: `ettercap`, `mitmproxy`, or custom Scapy scripts.

**Ethical note:** Only practice in a controlled lab environment. Never attack networks you don’t own.

### 6.4 Common CTF Challenges
- **Packet analysis (pcap):** Find a flag in a TCP stream, extract an image from HTTP, reconstruct a file from fragments.
- **Protocol reverse engineering:** A server sends a strange binary protocol; you must implement a client to get the flag.
- **Network pivoting:** Compromise a machine and use it to scan/attack internal networks.

**Exercise:** Use Wireshark to open a pcap from a CTF (e.g., from picoCTF “shark on wire” series). Practice filtering: `http`, `tcp.port==8080`.

---

## 🔨 MINI‑PROJECT: PORT SCANNER IN C AND PYTHON (1 HOUR)

### 7.1 Requirements
- Scan a range of ports on a given IP.
- TCP connect scan (try `connect()`; if it succeeds, port open).
- Timeout for responsiveness.
- Concurrent scanning (optional: using threads or `select`).

### 7.2 Python Version (Rapid Prototyping)
```python
import socket, sys

def scan_port(host, port):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(0.2)
        s.connect((host, port))
        s.close()
        return True
    except:
        return False

if __name__ == '__main__':
    host = sys.argv[1]
    start, end = map(int, sys.argv[2:4])
    for port in range(start, end+1):
        if scan_port(host, port):
            print(f"Port {port} open")
```
**Optimization:** Use `ThreadPoolExecutor` or `asyncio` to speed it up.

### 7.3 C Version (From Scratch)
Re‑implement in C, using `fork()` to create child processes that each scan a subset of ports. This combines networking and process management from Week 9.

**Exercise:** Add service fingerprinting: after connecting, read any banner sent, print it.

### 7.4 Extend to a Simple Recon Tool
- Resolve hostname to IP with `getaddrinfo`.
- Add a `--syn` flag that uses raw sockets (advanced).

---

## 📝 WEEK 11 PROBLEM SET (Daily 2‑4 Hours)

### PS11‑1: Network Programming in C
- **TCP Echo Server/Client:** Build a chat between two terminals using your echo server.
- **HTTP Client:** Write a C program that sends an HTTP GET request to a host given on command line and prints the response headers and body. Handle chunks properly.
- **Multiplexed Server:** Modify the echo server to handle up to 5 clients simultaneously using `fork()` or `select()`. Test with multiple netcat connections.

### PS11‑2: Packet Crafting with Scapy
- Write a Scapy script that sends a SYN scan to `127.0.0.1` ports 1‑1024 and prints open ports (based on SYN‑ACK response).
- Craft an ICMP echo request (ping) with a custom payload containing your flag string, and capture it with Wireshark to verify.

### PS11‑3: OverTheWire Bandit 28‑33
Finish Bandit. These levels involve `git`, more SSH, and some scripting. Document each.

### PS11‑4: picoCTF Forensics & Network
Solve at least **5 challenges** under “Forensics” (e.g., “shark on wire 1”, “extensions”, “WhitePages”) and “Network” categories. Use Wireshark, Python, and your network knowledge.

### PS11‑5: Reading – Kurose & Ross Chapter 3 (Transport Layer)
Read up to section 3.4 (principles of reliable data transfer). Focus on stop‑and‑wait, go‑back‑N, and TCP’s mechanisms. This will make the Stanford CS144 sequence numbers labs much clearer.

### PS11‑6: Watch Stanford CS144 Lecture 1
Watch the first lecture of Stanford CS144 (Introduction to Computer Networking). Take notes on the Internet architecture and the end‑to‑end principle.

### PS11‑7: Reflection
In `week11_reflection.md`:
1. What surprised you about how low‑level network communication really is?
2. Describe a time when your port scanner gave a false negative/positive. How did you adjust?
3. How can raw socket knowledge be misused? What ethical boundaries do you now understand better?
4. How does understanding the TCP handshake help you later in exploiting network‑based services?

---

## 🔥 DEEP DIVE WRAP‑UP

You’ve dismantled the network stack. You can create a socket, listen for connections, send crafted packets, and sniff traffic. You’ve used the tools of network reconnaissance (nmap, netcat), and built your own in C and Python. This is the foundation for everything from web exploitation to kernel network driver attacks.

Next week, we’ll enter the **OS phase**—reading xv6 source code and diving into operating system internals. The ability to spawn processes, handle files, and work with sockets will be crucial as you begin to understand system calls and kernel‑level privilege escalation.

**Open Wireshark, capture your own traffic, and marvel at the choreography of SYN, ACK, and data. You’re now the operator at every layer.**
