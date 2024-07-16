/*
ISA - Projekt
Generovani NetFlow dat ze zachycene sitove komunikace
Emma Krompascikova, xkromp00
November 2022
*/

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <pcap.h>
#include <netinet/ether.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <iostream>
#include <chrono>
#include <ctime> //for unix time
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAX_FLOW_COUNT 30

struct flow_key_t {
    uint32_t ip_dest;
    uint32_t ip_source;
    uint16_t port_source;
    uint16_t port_dest;
    uint8_t protocol_of_transport;
    uint8_t tos;
};

struct header_t {
    uint16_t version = 0;
    uint16_t count=0;
    uint32_t uptime = 0; //Current time in milliseconds since the export device started
    uint32_t unix_secs = 0;
    uint32_t unix_nsecs = 0;
    uint32_t flow_sequence = 0;
    uint8_t engine_type = 0; //ostane nula
    uint8_t engine_id = 0;//ostane nula
    uint16_t sampling_interval = 0;//ostane nula
};

struct flow_t { //o jednom toku
    uint32_t ip_source =0;
    uint32_t ip_dest=0;
    uint32_t next_hop_ip = 0; //ostane nula
    uint16_t input_interf_index = 0; //ostane nula
    uint16_t output_interf_index = 0; //ostane nula
    uint32_t number_of_packets=0;
    uint32_t number_of_bytes=0;
    uint32_t start_time=0;
    uint32_t end_time=0;
    uint16_t port_source=0;
    uint16_t port_dest=0;
    uint8_t pad=0; //ostane nula
    uint8_t tcp_flags =0;
    uint8_t protocol_of_transport=0;
    uint8_t tos=0;
    uint16_t source_as=0;//ostane nula
    uint16_t dest_as=0;//ostane nula
    uint8_t src_netmask_len=0;//ostane nula
    uint8_t dest_netmask_len=0;//ostane nula
    uint16_t padding=0;//ostane nula
    flow_key_t key;
};

struct flow_t_bez_key { //o jednom toku
    uint32_t ip_source =0;
    uint32_t ip_dest=0;
    uint32_t next_hop_ip = 0; //ostane nula
    uint16_t input_interf_index = 0; //ostane nula
    uint16_t output_interf_index = 0; //ostane nula
    uint32_t number_of_packets=0;
    uint32_t number_of_bytes=0;
    uint32_t start_time=0;
    uint32_t end_time=0;
    uint16_t port_source=0;
    uint16_t port_dest=0;
    uint8_t pad=0; //ostane nula
    uint8_t tcp_flags =0;
    uint8_t protocol_of_transport=0;
    uint8_t tos=0;
    uint16_t source_as=0;//ostane nula
    uint16_t dest_as=0;//ostane nula
    uint8_t src_netmask_len=0;//ostane nula
    uint8_t dest_netmask_len=0;//ostane nula
    uint16_t padding=0;//ostane nula
};

struct packet_t {
    header_t hlavicka_v_pakete;
    flow_t_bez_key flow;
};

struct timespec spec;

uint8_t flow_index = 0;
uint8_t flow_sequence = 1;
flow_t flows[MAX_FLOW_COUNT];
flow_t flows_closed[MAX_FLOW_COUNT];
flow_t_bez_key flows_bez_key[MAX_FLOW_COUNT];
uint8_t glob_poc_paket = 0;

void packet_handler(u_char *user, const struct pcap_pkthdr *h,const u_char *bytes);
void debugging_flow_key_t(struct flow_key_t debugging);
void extended_debugging_flow(struct flow_t debug);
bool porovnanie(struct flow_key_t prvy, struct flow_key_t druhy);
bool udpSend(const char *msg, char *ip_address, char *port);
void neaktivny(uint32_t moj, uint32_t posledny, uint8_t num_of_packets,const char *msg, char *ip_address, char *port,int hodnota_i);
void aktivny(uint32_t moj, uint32_t posledny, uint8_t num_of_packets,const char *msg, char *ip_address, char *port,int hodnota_a);



int main(int argc, char* argv[]){
    char *hodnota_f = (char*) "-"; //nazov suboru z ktoreho sa citaju pakety (- = STDIN)
    char hodnota_c[22] = "127.0.0.1:2055"; //ip add netflow kolektoru
    int hodnota_a = 60; //interval v sek, po kt sa exp aktiv zaznamy na kolektor
    int hodnota_i = 10; //interval v sek, po kt sa exp neaktiv zaznamy na kolektor
    int hodnota_m = 1024; //velkost flow cache, po dosazeni dojde k exp najstars zazn v chachi na kolektor
    int opt;

    while ((opt = getopt (argc, argv, "f:c:a:i:m:")) != -1){ //prepinace
        switch (opt){
            case 'f':
                hodnota_f = optarg;
            break;
            case 'c':
                strcpy(hodnota_c, optarg);
            break;
            case 'a':
                hodnota_a = atoi(optarg);
            break;
            case 'i':
                hodnota_i = atoi(optarg);
            break;
            case 'm':
                hodnota_m = atoi(optarg);
            break;
        }
    }

    char *ip_adreska = strtok(hodnota_c, ":");
    char* port = strtok(NULL, ":");

    pcap_t *fp;
    char errbuf[PCAP_ERRBUF_SIZE];

    fp = pcap_open_offline(hodnota_f, errbuf);
    if (fp == NULL) {
	    fprintf(stderr, "\npcap_open_offline() failed: %s\n", errbuf);
	    return -1;
    }

    if (pcap_loop(fp, 0, packet_handler, NULL) < 0) {
        fprintf(stderr, "\npcap_loop() failed: %s\n", pcap_geterr(fp));
        return -1;
    }

    /* for debugging
    for(int i=0; i<flow_index; i++){
        printf("============= %d\n", i);
        extended_debugging_flow(flows[i]);
    }
    */

    header_t header;
    packet_t packet;
    
    uint8_t pocitadlo = 0;
    uint32_t actual_time = 0;
    uint32_t time_of_last_packet  = 0;

    while(pocitadlo != flow_index){
        header.version=htons(5);
        header.count = htons(1);
    
        header.uptime = (flows[pocitadlo].start_time);
        clock_gettime(CLOCK_REALTIME, &spec);
        header.unix_secs = (flows[pocitadlo].start_time);
        header.flow_sequence = htonl(flow_sequence);
        header.unix_nsecs = htonl(spec.tv_nsec); // residual
        header.engine_type = 0;
        header.engine_id = 0;
        header.sampling_interval = 0;

        memcpy(&flows_bez_key[pocitadlo],&flows[pocitadlo],sizeof(flows[pocitadlo]));

        packet.hlavicka_v_pakete = header;
        packet.flow = flows_bez_key[pocitadlo];
        packet.flow.port_source=htons(packet.flow.port_source);
        packet.flow.port_dest=htons(packet.flow.port_dest);
        packet.flow.ip_source=htonl(packet.flow.ip_source);
        packet.flow.ip_dest=htonl(packet.flow.ip_dest);

        glob_poc_paket = packet.flow.number_of_packets;
        packet.flow.number_of_packets=htonl(packet.flow.number_of_packets);

        actual_time = packet.flow.end_time;
        time_of_last_packet = packet.flow.start_time;
        
        aktivny(actual_time, time_of_last_packet, glob_poc_paket,(char*)&packet, ip_adreska, port, hodnota_a );
        neaktivny(actual_time, time_of_last_packet, glob_poc_paket,(char*)&packet, ip_adreska, port, hodnota_i);
        udpSend((char*)&packet, ip_adreska, port);
        pocitadlo++;
        actual_time = packet.flow.end_time;
    }

        flow_sequence = 0;
        flow_index = 0;
    return 0;
}

void packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes){
    struct flow_key_t current_flow_key;
    ether_header *ethernet = (ether_header*)bytes;
   
    char dest_mac[18] = { 0 };  //mac adresy destinacion

    char source_mac[18] = { 0 };  //mac adresy source

    snprintf(source_mac, 18, "%02x:%02x:%02x:%02x:%02x:%02x", ethernet->ether_shost[0],ethernet->ether_shost[1],ethernet->ether_shost[2],ethernet->ether_shost[3],ethernet->ether_shost[4],ethernet->ether_shost[5]);

    ethernet++; //posuvame sa na ip hlavicku
    struct iphdr* ipv4 = (struct iphdr*)ethernet;

    char* ipv4_source = inet_ntoa(in_addr {ipv4->saddr } ); //ip zdroj
    current_flow_key.ip_source = ipv4->saddr;

    char* ipv4_dest = inet_ntoa(in_addr {ipv4->daddr } ); //ip destinacia
    current_flow_key.ip_dest = ipv4->daddr;

    uint8_t protocol = ipv4->protocol;
    current_flow_key.protocol_of_transport = protocol;
    
    uint8_t type_of_service = ipv4->tos; //vytiahneme tos
    current_flow_key.tos = type_of_service;
    
    unsigned int header_length = (ipv4->ihl)*4; //na zistenie dlzky hlavky vythianeme ihl a vynasob 4mi
    char *pointer_transport = ((char*)ipv4) + header_length;

    if (protocol == 17){ //17 je UDP
        udphdr *hlavicka_udp = (udphdr*)(pointer_transport);
        uint16_t udp_source = hlavicka_udp->source;
        uint16_t udp_destination = hlavicka_udp->dest;
        uint16_t udp_dlzka = hlavicka_udp->len;
        current_flow_key.port_dest = udp_destination;
        current_flow_key.port_source = udp_source;
    }
    else if(protocol == 6){ //6 je TCP
        tcphdr *hlavicka_tcp = (tcphdr*)(pointer_transport);
        uint16_t tcp_source = hlavicka_tcp->source;
        uint16_t tcp_destination = hlavicka_tcp->dest;
        current_flow_key.port_dest=tcp_destination;
        current_flow_key.port_source=tcp_source;
    }
    else if(protocol == 1){ //1 je ICMP
        icmp *hlavicka_icmp = (icmp*)(pointer_transport);
        current_flow_key.port_dest=0;
        current_flow_key.port_source=0;
    }

    flow_t new_flow;
    new_flow.key = current_flow_key;
    
    new_flow.port_dest = htons(current_flow_key.port_dest);
    new_flow.port_source = htons(current_flow_key.port_source);
    new_flow.ip_source = htonl(current_flow_key.ip_source);
    new_flow.ip_dest = htonl(current_flow_key.ip_dest);
    new_flow.protocol_of_transport = current_flow_key.protocol_of_transport;
    new_flow.tos = current_flow_key.tos;
    new_flow.pad = 0;
    new_flow.padding = htons(0);
    new_flow.next_hop_ip = 0;
    new_flow.input_interf_index = 0;
    new_flow.output_interf_index = 0;
    
    new_flow.src_netmask_len = 0;
    new_flow.dest_netmask_len = 0;
    new_flow.source_as = 0;
    new_flow.dest_as = 0;

    if(flow_index == 0){ //urcite pridame novy flow
        new_flow.start_time = htonl(h->ts.tv_sec);
        new_flow.end_time = htonl(h->ts.tv_sec);
        new_flow.number_of_packets = htonl(flows[flow_index].number_of_packets);
        flows[flow_index].number_of_bytes = htonl(h->caplen - 14);
        flows[flow_index] = new_flow;
        flow_index++;   
    }
    else{
        new_flow.end_time = htonl(h->ts.tv_sec);
        for(int i=0; i<flow_index; i++){
            bool suRovnake = porovnanie(flows[i].key,new_flow.key);
            if (suRovnake) { //nasli sme existujuci flow, nepridavame novy
                flows[i].end_time = htonl(h->ts.tv_sec);
                flows[i].number_of_packets++;
                flows[i].number_of_bytes += htonl(h->caplen - 14);
                break;
            }
            if (i == flow_index - 1) { //nenasli sme existujuci flow, pridavame novy
                new_flow.start_time = htonl(h->ts.tv_sec);
                new_flow.end_time = htonl(h->ts.tv_sec);
                flows[flow_index] = new_flow;
                flow_index++;
            }
        }
    }
    flow_t_bez_key new_flow_bez_key;
    memcpy(&new_flow_bez_key,&new_flow,sizeof(struct flow_t_bez_key));
}

void debugging_flow_key_t(struct flow_key_t debugging){
    char* ipv4_dest = inet_ntoa(in_addr {debugging.ip_dest } );
    printf("Dest IP %s\n",ipv4_dest);
    char* ipv4_source = inet_ntoa(in_addr {debugging.ip_source } );
    printf("Source IP %s\n",ipv4_source);
    printf("Port dest %d\n", ntohs(debugging.port_dest)); //ntohs na prehodenie big endian na little endian
    printf("Port source %d\n", ntohs(debugging.port_source));
    printf("Prot of trans %d\n", debugging.protocol_of_transport);
    printf("TOS %d\n", debugging.tos);
}

void extended_debugging_flow(struct flow_t debug){
    time_t start_time = debug.start_time;
    time_t end_time = debug.end_time;
    debugging_flow_key_t(debug.key);
    printf("Numb of bytes %d\n", debug.number_of_bytes);
    printf("Numb of packets %d\n", debug.number_of_packets);
    printf("Start time %s\n", ctime(&start_time));
    printf("End time %s\n", ctime(&end_time));
}

bool porovnanie(struct flow_key_t prvy, struct flow_key_t druhy){
    bool suRovnake;
    if(prvy.ip_dest == druhy.ip_dest &&
        prvy.ip_source == druhy.ip_source &&
        prvy.port_dest == druhy.port_dest &&
        prvy.port_source == druhy.port_source &&
        prvy.protocol_of_transport == druhy.protocol_of_transport &&
        prvy.tos == druhy.tos)
    {   
        suRovnake = true;
    }
    else{
        suRovnake = false;
    }
    return suRovnake;
}

bool udpSend(const char *msg, char *ip_address, char *port){
    // setup socket file descriptor
    sockaddr_in servaddr;

    packet_t *paket = (packet_t*)msg;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0){
        perror("Error opening socket");
        return false;
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip_address);
    servaddr.sin_port = htons(atoi(port));

    // send packet and check for error
    if (sendto(fd, msg, sizeof(struct packet_t), 0, (sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("Error sending netflow packet");
        close(fd);
        return false;
    }
    else {
        close(fd); //poslanie sa vykonalo
        return true;
    }
}

void neaktivny(uint32_t moj, uint32_t posledny, uint8_t num_of_packets,const char *msg, char *ip_address, char *port,int hodnota_i){
    for (int i=0;i < num_of_packets;i++){
        if (moj - posledny > hodnota_i){
            //exportujem
            //udpSend((char*)&msg, ip_address, port);
            flow_sequence = 0;
        }
    }
}

void aktivny(uint32_t moj, uint32_t posledny, uint8_t num_of_packets,const char *msg, char *ip_address, char *port,int hodnota_a){
    for (int i=0;i < num_of_packets;i++){
        if (moj - posledny > hodnota_a){
            //exportujem
            //udpSend((char*)&msg, ip_address, port);
            flow_sequence = 0;
        }
    }
}

