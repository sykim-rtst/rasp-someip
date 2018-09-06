#include <stdio.h>
#include <someip/someip.h>
#include "someip-sd.h"

int send_offer_service(someip_offering_service_t *offer)
{
    return -1;
}

void handle_someip_packet(struct netbuf *buf)
{

    return;

}


void handle_findservice(ip_addr_t *addr, unsigned short port, char *data_ptr, unsigned long entries_len)
{
    someip_sd_entry_t *entry = (someip_sd_entry_t *)data_ptr;
    someip_offering_service_t *offer;

    offer = someip_find_offering_service(entry->t1.service_id, entry->t1.instance_id);

    if(!offer) {
        return;
    }

    send_offer_service(offer);

}


void handle_offerservice(ip_addr_t *addr, unsigned short port, char *data_ptr, unsigned long entries_len)
{
    someip_sd_entry_t *entry = (someip_sd_entry_t *)data_ptr;
    someip_offering_service_t *offer = (someip_offering_service_t *)malloc(sizeof(someip_offering_service_t));
    char *ptr;

    offer->service_id = entry ->t1.service_id;
    offer->instance = entry->t1.instance_id;

    //printf("service id: %04x instance id: %04x\r\n",offer->service_id, offer->instance);
    unsigned int opt1_idx = entry->t1.idx_1st_opt;
    unsigned int opt1_num = entry->t1.num_opts >> 4;

    ptr = data_ptr + entries_len + 4;

    int i = 0;

    //printf("opt index: %d num: %d\r\n",opt1_idx,opt1_num);
    for(i = 0; i < opt1_idx; ++i) {
        ptr += *((uint16_t *)ptr) + 3;
    }

    offer->ipv4_addr = ((uint32_t *)ptr)[1];
    offer->port = ((uint16_t *)ptr)[5];

    uint8_t *ip = (uint8_t *)(&(offer->ipv4_addr));
    //printf("ipv4 addr: %d.%d.%d.%d port: %lu\r\n",ip[0], ip[1], ip[2], ip[3], offer->port);

    someip_add_offering_service(offer);

    someip_requested_service_t *req = someip_find_req_service(offer->service_id, 0xffff, offer->instance);

    if(req) {
        req->offer = offer;

        if(!req->availabiltity) {
            req->availabiltity = 1;

            if(req->avail_handler) {
                req->avail_handler(offer->service_id, offer->instance, 1);
            }
        }
    }
}


void handle_someip_sd_packet(struct netbuf *buf)
{
    unsigned char *recv_data, *data_ptr;
    ip_addr_t *addr;
    unsigned short port;
    unsigned long entries_len, len;

    someip_sd_header_t *sd_header;

    addr = netbuf_fromaddr(buf);
    port = netbuf_fromport(buf);
    netbuf_data(buf, &recv_data, &len);

    /*
    printf("\r\n(1) Received data %d\r\n", len);

    int i;
    for(i=0;i<len && i<90;++i){
        if(i % 8 == 0)
            printf("\r\n");
        printf("%02x ",recv_data[i]);
    }
    */

    sd_header = (someip_sd_header_t *)recv_data;

    if(sd_header->proto_ver != 0x01 || sd_header->if_ver != 0x01 ||
            sd_header->msg_type != 0x02 || sd_header->ret_code != 0x00) {
        printf("this packet may not be a someip-sd\r\n");
        return;
    }

    data_ptr = recv_data + sizeof(someip_sd_header_t);
    entries_len = ntohl(*((unsigned long *)data_ptr));

    data_ptr += sizeof(unsigned long);

    while(entries_len) {
        switch(*data_ptr ) {
            case 0x00:
                printf("Got findservice\r\n");
                handle_findservice(addr, port, data_ptr, entries_len);
                break;

            case 0x01:
                printf("Got Offerservice\r\n");
                handle_offerservice(addr, port, data_ptr, entries_len);
                break;

            default:
                printf("unsupported entry type\r\n");
        }

        entries_len -= sizeof(someip_sd_entry_t);
        data_ptr += sizeof(someip_sd_entry_t);
    }

}
