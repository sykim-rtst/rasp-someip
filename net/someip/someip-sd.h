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
} someip_sd_entry_t

typedef struct someip_sd_entry_array_s {
    uint32_t    length;
    someip_sd_entry_t   entris[0];
} __attribute__((packed)) someip_sd_entry_array_t


typedef struct someip_sd_opt_array_s {
    uint32_t    length;
    uint8_t     entris[0];
} __attribute__((packed)) someip_sd_opt_array_t


#endif
