#include <types.h>
#include <lwip/sockets.h>
#include <ethif.h>
#include <netif/etharp.h>
#include <lwip/sys.h>

#ifndef __SOMEIP_H__
#define __SOMEIP_H__


typedef struct someip_header_s {
    uint32_t    msg_id;
    uint32_t    length;
    uint32_t    req_id;
    uint8_t     protocol_ver;
    uint8_t     interface_ver;
    uint8_t     msg_type;
    uint8_t     ret_code;
    uint8_t     payload[0];
} __attribute__((packed)) someip_t;

enum someip_state {
    ST_REGISTERED
};

typedef struct someip_app_s {
    uint16_t    client_id;
    void        (*state_handler)(int state);
    int         state;
} someip_app_t;

typedef uint16_t service_t;
typedef uint16_t client_t;
typedef uint16_t instance_t;
typedef uint16_t method_t;
typedef void    *someip_req_t;

someip_app_t *someip_register_app(client_t my_id);
int someip_register_msg_handler(client_t my_id, service_t service_id,
                                instance_t instance, method_t method, void (*msg_handler)(someip_t *someip));
int someip_unregister_msg_handler(service_t my_id, service_t service_id,
                                  instance_t instance, method_t method);

void Someip_Init();
void Someip_MainFunction();
void Someip_RxIndication(PduIdType RxPduId, const PduInfoType *PduData);

#endif
