#include <types.h>

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
} __attribute__((packted)) someip_t;

enum someip_state {
    ST_REGISTERED;
}

typedef struct someip_service_s {
    uint16_t    service_id;
    uint16_t    instance_id;
    void        (*state_handler)(int state);
    int         state;
} someip_service_t;

typedef uint16_t service_t;
typedef uint16_t instance_t;
typedef uint16_t method_t;

someip_service_t *someip_register_service(service_t service_id, instance_t instance);
int someip_register_msg_handler(service_t my_id, service_t service_id,
                                instance_t instance, method_t method, void (*msg_handler)(someip_t *someip));
int someip_unregister_msg_handler(service_t my_id, service_t service_id,
                                  instance_t instance, method_t method);
int someip_register_avail_handler(service_t my_id, service_t service_id,
                                  instance_t instance, void (*avail_handler)(service_t service, instance_t instance, int available));
int someip_unregister_avail_handler(service_t my_id, service_t service_id,
                                    instance_t instance);
int someip_register_state_handler(service_t my_id, service_t service_id,
                                  instance_t instance, void (*state_handler)(int state));
int someip_unregister_state_handler(service_t my_id, service_t service_id, instance_t instance);
int offer_service(service_t my_id, service_t service_id, instance_t instance);
int stop_offer_service(service_t my_id, service_t service_id, instance_t instance);
int request_service(someip_service_t my_id, service_t service_id, instance_t instance,
                    void (*avail_handler)(service_t service, instance_t instance, int available));
int release_service(someip_service_t my_id, service_t service_id, instance_t instance);

#endif
