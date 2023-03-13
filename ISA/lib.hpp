// Samuel Kuchta (xkucht11)

#pragma once

#define __FAVOR_BSD

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>  // The pipe() function will create a pipe and place two file descriptors, one each into the arguments
#include <sys/socket.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <cstring> // similar to normal strings, but they contain an extra null character at the end of the string that makes them Cstrings.
#include <thread>
#include <chrono>
#include <pcap.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

#include<netdb.h>
#include<err.h>

#include <arpa/inet.h>      // inet_ntop()
#define __FAVOR_BSD
#include <netinet/in.h>
#define __FAVOR_BSD
#include <netinet/ip.h>
#define __FAVOR_BSD
#include <netinet/tcp.h>
#define __FAVOR_BSD
#include <netinet/udp.h>
#define __FAVOR_BSD
#include <netinet/ip_icmp.h>

using namespace std;

  // makrá
#define BUFFER 72  // buffer length 
#define IPv4_Verzia 2048
#define ETH_LEN 14

#define USAGE(_prog_name) \
  cerr << "Použítí: " << _prog_name << " [-f <file>] [-c <netflow_collector>[:<port>]] [-a <active_timer>] [-i <inactive_timer>] [-m <count>]\nSpusťí NetFlow exporter, ktori robi NetFlow zaznamy z pcap suborov, a priebezne ich odosiela na NetFlow kolektor.\n\n";  \
  return 1;

  // struktury
  typedef struct NetFlow_5_Hdr {
    uint16_t version;           // NetFlow export format version number 
    uint16_t count;             // Number of flows exported in this packet (1-30)

    uint32_t SysUptime;         // Current time in milliseconds since the export device booted 
    uint32_t unix_secs;         // Current count of seconds since 0000 UTC 1970 
    uint32_t unix_nsecs;        // Residual nanoseconds since 0000 UTC 1970 
    uint32_t flow_sequence;     // Sequence counter of total flows seen 

    uint8_t  engine_type;       // Type of flow-switching engine 
    uint8_t  engine_id;         // Slot number of the flow-switching engine

    uint16_t sampling_interval; // First two bits hold the sampling mode; remaining 14 bits hold value of sampling interval
  } NF5_H;

  typedef struct NetFlow_5_Flow {
    uint32_t srcaddr;   // Source IP address
    uint32_t dstaddr;   // Destination IP address
    uint32_t nexthop;   // IP address of next hop router

    uint16_t input;     // SNMP index of input interface
    uint16_t output;    // SNMP index of output interface

    uint32_t dPkts;     // Packets in the flow
    uint32_t dOctets;   // Total number of Layer 3 bytes in the packets of the flow
    uint32_t First;     // SysUptime at start of flow
    uint32_t Last;      // SysUptime at the time the last packet of the flow was received

    uint16_t srcport;   // TCP/UDP source port number or equivalent 
    uint16_t dstport;   // TCP/UDP destination port number or equivalent

    uint8_t pad1;       // Unused (zero) bytes
    uint8_t tcp_flags;  // Cumulative OR of TCP flags
    uint8_t prot;       // IP protocol type (for example, TCP = 6; UDP = 17)
    uint8_t tos;        // IP type of service (ToS)

    uint16_t src_as;    // Autonomous system number of the source, either origin or peer
    uint16_t dst_as;    // Autonomous system number of the destination, either origin or peer
    
    uint8_t src_mask;   // Source address prefix mask bits
    uint8_t dst_mask;   // Destination address prefix mask bits

    uint16_t pad2;      // Unused (zero) bytes
  } NF5_F;


  // funkcie
void netFlow_analyze(unsigned char* user, struct pcap_pkthdr* hlavicka_paketu, unsigned char* packet_ptr);

void check_full();  // if Netflow cache full, send everything to collector, clear whole buffer.
void export_timeout();
int check_key(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port, uint8_t protocol, uint8_t ToS);
void add_new_flow(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port, uint8_t protocol, uint8_t ToS, uint8_t tcp_flags, uint32_t IHL, uint32_t mikro_cas);
void update_key(int row, uint8_t tcp_flags, uint32_t IHL, uint32_t mikro_cas);
void delete_flow(int row);

void check_remaining();

int send_netflow_to_collector(int row);