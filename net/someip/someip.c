#include <stdio.h>
#include <someip/someip.h>
#include "someip-sd.h"

someip_service_t *someip_register_service(service_t my_id, instance_t instance)
{
    someip_service_t *srv;

    srv = (someip_service_t *)malloc(sizeof(someip_service_t));

    if(!srv) {
        return NULL;
    }

    srv->service_id = my_id;
    srv->instance = instance;
    return srv;
}

int someip_register_msg_handler(service_t my_id, service_t service_id,
                                instance_t instance, method_t method, void (*msg_handler)(someip_t *someip))
{
    int err;

    err = someip_find_service(my_id, service_id, instance, method, msg_handler);

    return err;
}

int someip_unregister_msg_handler(service_t my_id, service_t service_id,
                                  instance_t instance, method_t method);
{
    int err;
    msg_handler_list_t *handler;

    handler = someip_find_msg_handler(service_id, my_id, instance, method);

    if(!handler) {
        return RTWORKS_ERRNO_NOTFOUND;
    }

    someip_del_msg_handler(handler);

    return 0;
}

int someip_register_state_handler(service_t my_id, service_t service_id,
                                  instance_t instance, void (*state_hander)(int state));
{
    int err;
    someip_requeted_service_t *req;

    req = someip_find_service(service_id, my_id, instance);

    if(!req) {
        return RTWORKS_ERRNO_NOTFOUND;
    }

    req -> state_hander = state_handler;

    return 0;
}
int someip_unregister_state_handler(service_t my_id, service_t service_id, instance_t instance);
{
    int err;
    someip_requeted_service_t *req;

    req = someip_find_service(service_id, my_id, instance);

    if(!req) {
        return RTWORKS_ERRNO_NOTFOUND;
    }

    req -> state_hander = state_handler;

    return 0;
}
int offer_service(service_t my_id, service_t service_id, instance_t instance);
int stop_offer_service(service_t my_id, service_t service_id, instance_t instance);
int request_service(someip_service_t my_id, service_t service_id, instance_t instance,
                    void (*avail_handler)(service_t service, instance_t instance, int available) )
{
    someip_reqested_service_t *srv;

    srv = (someip_service_t *)malloc(sizeof(someip_requested_service_t));

    if(!srv) {
        return NULL;
    }

    srv->req = my_id;
    srv->service_id = service_id;
    srv->instance = instance;
    srv->avail_handler = avial_handler;

    return someip_add_req_service(srv);

}

int release_service(service_t my_id, service_t service_id, instance_t instance);
