// Samuel Kuchta (xkucht11)
#include "lib.hpp"

extern string collector_dst_ip;
extern string collector_dst_port;
extern int pocet;
extern NF5_H flow_hdr;
extern NF5_F** pole_F;
extern int count;
extern uint32_t active_timer;
extern uint32_t seconds;
extern uint32_t boot_cas;
extern uint32_t boot_mikro_cas;

void netFlow_analyze(u_char* user, struct pcap_pkthdr* packet_hdr, u_char* packet_ptr) {
  struct ether_header*  eth_hdr = (struct ether_header *)packet_ptr;
  struct ip*             ip_hdr = (struct ip*)          (packet_ptr + ETH_LEN);
  struct tcphdr* tcp_hdr = (struct tcphdr*)(packet_ptr + ETH_LEN + ip_hdr->ip_hl*4);
  struct udphdr* udp_hdr = (struct udphdr*)(packet_ptr + ETH_LEN + ip_hdr->ip_hl*4);

  bool tcp = false;
  bool udp = false;

  int row;

    // zisti verziu ip
  int packet_type = ((int)(packet_ptr[12]) << 8) | (int) packet_ptr[13];
  if (packet_type != IPv4_Verzia) {
    return;  // zachytavame len ipv4 komunikaciu (Netflow v5)
  }

  if (ip_hdr->ip_p == IPPROTO_TCP) {
    tcp = true;
  } else if (ip_hdr->ip_p == IPPROTO_UDP) {
    udp = true;
  } else if (ip_hdr->ip_p == IPPROTO_ICMP) {
    //
  } else {
    return;
  }

  uint32_t cas = packet_hdr->ts.tv_sec;  /* timestamp seconds */
  uint32_t mikro_cas = packet_hdr->ts.tv_usec;  /* timestamp microseconds */

  if (flow_hdr.flow_sequence == 0 && pocet == 0) {
    uint32_t boot_cas = cas;  /* timestamp seconds */
    uint32_t boot_mikro_cas = mikro_cas;  /* timestamp microseconds */
  }

  flow_hdr.SysUptime = (boot_mikro_cas - mikro_cas) / 1000; // milisec
  flow_hdr.unix_secs = cas; // sec
  flow_hdr.unix_nsecs = mikro_cas * 1000; // nanosec


  // packet_hdr  -> cas, interface
  // ethernet header -> ip alebo icmp
  // ip -> srcip, dstip, Tos, velkost ip (IHL)
  // tcp/udp -> srcport, dstport, flagy
  
  uint32_t src_ip = ip_hdr->ip_src.s_addr;
  uint32_t dst_ip = ip_hdr->ip_dst.s_addr;
  uint8_t protocol = ip_hdr->ip_p;
  uint8_t ToS = ip_hdr->ip_tos;
  uint32_t IHL = ntohl(ip_hdr->ip_len) - ip_hdr->ip_hl;
  uint8_t tcp_flags = 0;
  uint16_t src_port, dst_port;

  if (tcp == true) {
    src_port = tcp_hdr->th_sport;
    dst_port = tcp_hdr->th_dport;
    tcp_flags = tcp_hdr->th_flags;
    // tcp_hdr->fin | tcp_hdr->syn << 1 | tcp_hdr->rst << 2 | tcp_hdr->psh << 3 | tcp_hdr->ack << 4 | tcp_hdr->urg << 5;
  } else if (udp == true) {
    src_port = udp_hdr->uh_sport;
    dst_port = udp_hdr->uh_dport;
  } else {
    // ICMP
  }

  check_full();
  export_timeout();
  if ((row = check_key(src_ip, dst_ip, src_port, dst_port, protocol, ToS)) == -1) {
    add_new_flow(src_ip, dst_ip, src_port, dst_port, protocol, ToS, tcp_flags, IHL, mikro_cas);
  } else {
    update_key(row, tcp_flags, IHL, mikro_cas);
  }
}

void check_full() {
  if (pocet >= count) {
    for (int i = 0; i < count; i++) {
      delete_flow(i);
    }
  }
}

void export_timeout() { 
  for (int i = 0; i < count; i++) {
    uint16_t time_passed = flow_hdr.SysUptime - pole_F[i]->Last;
    if ((pole_F[i]->prot == 6 && (time_passed > active_timer || pole_F[i]->tcp_flags & TH_FIN != 0))
    || (pole_F[i]->prot == 17 && time_passed > seconds)) {
      delete_flow(i);
    }
  }
}

int check_key(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port, uint8_t protocol, uint8_t ToS)  {
  for (int i = 0; i < count; i++) {
    if (src_ip == pole_F[i]->srcaddr && dst_ip == pole_F[i]->dstaddr &&
        src_port == pole_F[i]->srcport && dst_port == pole_F[i]->dstport &&
        protocol == pole_F[i]->prot && ToS == pole_F[i]->tos) {
      return i;  // hodnota kde sa nachadza flow
    }
  }
  return -1;  // nenaslo sa
}

void add_new_flow(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port, uint8_t protocol, uint8_t ToS, uint8_t tcp_flags, uint32_t IHL, uint32_t mikro_cas) {
  for (int i = 0; i < count; i++) {
    if (pole_F[i]->srcaddr == 0) {
        
      pole_F[i]->srcaddr = src_ip;  
      pole_F[i]->dstaddr = dst_ip; 

      pole_F[i]->dPkts = htonl(1);
      pole_F[i]->dOctets = htonl(IHL);

      pole_F[i]->First = htonl(mikro_cas / 1000); // firstpkt time  // sysuptime
      pole_F[i]->Last = pole_F[i]->First;

      pole_F[i]->srcport = src_port; 
      pole_F[i]->dstport = dst_port; 

      pole_F[i]->tcp_flags |= tcp_flags; 
      pole_F[i]->prot = protocol;  
      pole_F[i]->tos = ToS; 
      
      pole_F[i]->src_mask = -1;
      pole_F[i]->dst_mask = -1;
      pocet++;
      return;
    }
  }
}

void update_key(int row, uint8_t tcp_flags, uint32_t IHL, uint32_t mikro_cas) {
  pole_F[row]->dPkts = ntohl(pole_F[row]->dPkts);
  pole_F[row]->dPkts++;
  pole_F[row]->dPkts = htonl(pole_F[row]->dPkts);

  pole_F[row]->dOctets = ntohl(pole_F[row]->dOctets);
  pole_F[row]->dOctets += IHL; // += ip_ihl;
  pole_F[row]->dOctets = htonl(pole_F[row]->dOctets);

  pole_F[row]->Last = htonl(mikro_cas / 1000); // // sysuptime
  pole_F[row]->tcp_flags |= tcp_flags;
}

void delete_flow(int row) {
  send_netflow_to_collector(row);
  pole_F[row]->srcaddr = 0;
  pocet--;
}

void check_remaining() {
  if (pocet != 0) {
    for (int i = 0; i < count; i++) {
      if (pole_F[i]->srcaddr != 0) {
        delete_flow(i);
      }
    }
  }
}

int send_netflow_to_collector(int row) { // posli header a flow na kolektor
  int sock;                  // socket descriptor
  int i;
  struct sockaddr_in server; // address structures of the server and the client
  struct hostent* servent;   // network host entry required by gethostbyname()       
  char buffer[BUFFER];            

  memset(&server, 0, sizeof(server)); // erase the server structure
  server.sin_family = AF_INET;                   

  // make DNS resolution of the first parameter using gethostbyname()
  if ((servent = gethostbyname(collector_dst_ip.c_str())) == NULL) {  // check the first parameter
    errx(1,"gethostbyname() failed\n");
  }
  memcpy(&server.sin_addr,servent->h_addr,servent->h_length);  // copy the first parameter to the server.sin_addr structure

  server.sin_port = htons(atoi(collector_dst_port.c_str()));        // server port (network byte order)
   
  if ((sock = socket(AF_INET , SOCK_DGRAM , 0)) == -1) {   //create a client socket
    err(1,"socket() failed\n");
  }
             
  if (connect(sock, (struct sockaddr *)&server, sizeof(server))  == -1) {
    err(1, "connect() failed");
  }

  flow_hdr.flow_sequence = htons(flow_hdr.flow_sequence);

  memcpy(buffer, &flow_hdr, sizeof(NF5_H));
  memcpy(buffer+24, pole_F[row], sizeof(NF5_F));

  flow_hdr.flow_sequence = ntohs(flow_hdr.flow_sequence);
  flow_hdr.flow_sequence++;

  i = send(sock, buffer, 72, 0);     // send data to the server
  if (i == -1) {  // check if data was sent correctly
    err(1,"send() failed");
  } else if (i != 72) {
    err(1,"send(): buffer written partially");
  }

  close(sock);
  return 0;
}
