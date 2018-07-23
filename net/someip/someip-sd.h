#include <types.h>

#ifndef __SOMEIP_SD_H__
#define __SOMEIP_SD_H__

typedef struct someip_sd_header_s {
    uint32_t    req_id;
    uint8_t     proto_ver;
    uint8_t     if_ver;
    uint8_t     msg_type;
    uint8_t     ret_code;
    uint32_t     flags;
} __attribute__((packted)) someip_sd_header_t;

typedef union {
    typedef struct {
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
    } __attribute__((packed)) someip_sd_entry_t1;

    typedef struct {
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
    } __attribute__((packed)) someip_sd_entry_t2;

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

typedef struct someip_requested_service_s {
    struct someip_requiested_service_list_s *next;
    someip_service_t    req;
    uint16_t    serivice_id;
    uint16_t    instance;
    void (*avail_handler)(service_t service, instance_t instance, int available);
    int availabiltity;
} someip_requeted_service_t;

typedef struct msg_handler_list_s {
    struct msg_handler_list_s *next;
    uint16_t    req_id;
    uint16_t    serivice_id;
    uint16_t    instance;
    uint16_t    method;
    void (*msg_hander)(someip_t *someip);
} msg_handler_list_t;

int someip_add_req_service(someip_requeted_service_t *service);
someip_requeted_service_t *someip_find_req_service(uint16_t service_id, uint16_t req_id, uint16_t instance);
int someip_del_req_service(someip_requeted_service_t *service);

int someip_add_msg_handler(service_t my_id, service_t service_id,
                           instance_t instance, method_t method, void (*msg_handler)(someip_t *someip))
msg_handler_list_t *someip_find_msg_handler(uint16_t service_id, uint16_t req_id,
        uint16_t instance, uint16_t method);
int someip_del_msg_handler(msg_handler_list_t *msg_del);
#endif
