#include <types.h>
#include <lwip/arch/sys_arch.h>
#include <lwip/opt.h>
#include <lwip/mem.h>
#include <lwip/memp.h>
#include <lwip/def.h>
#include <lwip/pbuf.h>
#include <lwip/sys.h>
#include <lwip/tcpip.h>
#include <lwip/api.h>
#include <lwip/netif.h>
#include <lwip/ip_addr.h>
#ifndef __SOMEIP_SD_H__
#define __SOMEIP_SD_H__

typedef struct someip_sd_header_s {
    uint32_t    msg_id;
    uint32_t    length;
    uint32_t    req_id;
    uint8_t     proto_ver;
    uint8_t     if_ver;
    uint8_t     msg_type;
    uint8_t     ret_code;
    uint32_t     flags;
} __attribute__((packed)) someip_sd_header_t;

typedef union {
    struct {
        uint8_t     tyoe;
        uint8_t     idx_1st_opt;
        uint8_t     idx_2nd_opt;
        uint8_t     num_opts;
        uint16_t    service_id;
        uint16_t    instance_id;
        uint8_t     maj_ver;
        uint8_t     ttl_low;
        uint16_t    ttl_high;
        uint32_t    min_ver;
    } __attribute__((packed)) t1;

    struct {
        uint8_t     tyoe;
        uint8_t     idx_1st_opt;
        uint8_t     idx_2nd_opt;
        uint8_t     num_opts;
        uint16_t    service_id;
        uint16_t    instance_id;
        uint8_t     maj_ver;
        uint8_t     ttl_low;
        uint16_t    ttl_high;
        uint16_t    counter;
        uint16_t    eventgrp_id;
    } __attribute__((packed)) t2;

    unsigned char entry[16];
} someip_sd_entry_t;

typedef struct someip_sd_entry_array_s {
    uint32_t    length;
    someip_sd_entry_t   entris[0];
} __attribute__((packed)) someip_sd_entry_array_t;


typedef struct someip_sd_opt_array_s {
    uint32_t    length;
    uint8_t     entris[0];
} __attribute__((packed)) someip_sd_opt_array_t;


typedef struct someip_offering_service_s {
    struct someip_offering_service_s *next;
    uint8_t     type;
    uint16_t    service_id;
    uint16_t    instance;
    uint64_t    expired_time;
    uint8_t     endpt_type;
    uint32_t   ipv4_addr;
    uint16_t    port;
    void       *misc_opts;
} someip_offering_service_t;

typedef struct someip_requested_service_s {
    struct someip_requested_service_s *next;
    someip_service_t    *req;
    uint16_t    service_id;
    uint16_t    instance;
    void (*avail_handler)(service_t service, instance_t instance, int available);
    struct someip_offering_service_s *offer;
    int availabiltity;
} someip_requested_service_t;

typedef struct msg_handler_list_s {
    struct msg_handler_list_s *next;
    uint16_t    req_id;
    uint16_t    service_id;
    uint16_t    instance;
    uint16_t    method;
    void (*msg_handler)(someip_t *someip);
} msg_handler_list_t;

int someip_add_offering_service(someip_offering_service_t *service);
someip_offering_service_t *someip_find_offering_service(uint16_t service_id, uint16_t instance);
int someip_del_offering_service(someip_offering_service_t *service);

int someip_add_req_service(someip_requested_service_t *service);
someip_requested_service_t *someip_find_req_service(uint16_t service_id, uint16_t req_id, uint16_t instance);
int someip_del_req_service(someip_requested_service_t *service);

int someip_add_msg_handler(service_t my_id, service_t service_id,
                           instance_t instance, method_t method, void (*msg_handler)(someip_t *someip));
msg_handler_list_t *someip_find_msg_handler(uint16_t service_id, uint16_t req_id,
        uint16_t instance, uint16_t method);
int someip_del_msg_handler(msg_handler_list_t *msg_del);


void handle_someip_packet(struct netbuf *buf);
void handle_someip_sd_packet(struct netbuf *buf);

#endif
