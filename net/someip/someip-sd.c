#include <stdio.h>
#include <someip/someip.h>
#include "someip-sd.h"

msg_handler_list_t msg_head;
someip_requested_service_t srv_head;

int someip_add_req_service(someip_requeted_service_t *service)
{
    someip_reqested_service_t *srv = &srv_head;

    if(someip_find_req_service(service->service_id, service->req->service_id,
                               service->instance)) {
        return 0;
    }

    service->next = srv->next;
    srv->next = service;

    return 0;
}


someip_requeted_service_t *someip_find_req_service(uint16_t service_id,
        uint16_t req_id, uint16_t instance)
{

    someip_reqested_service_t *srv = srv_head.next;

    while(srv) {
        if(srv->service_id == service_id &&
                srv->req->service_id == my_id &&
                srv->instance == instance) {
            return srv;
        }

        srv = srv->next;
    }

    return srv;
}


int someip_add_msg_handler(service_t my_id, service_t service_id,
                           instance_t instance, method_t method, void (*msg_handler)(someip_t *someip))
{
    msg_handler_list_t *msg = &msg_head;
    msg_handler_list_t *new_m;
    new_m = someip_find_msg_handler(service_id, my_id, instance, method);

    if(new_m) {
        new_m->msg_handler = msg_handler;
        return 0;
    }

    new_m = (msg_handler_list_t *)malloc(sizeof(msg_handler_list_t));

    new_m->req_id = my_id;
    new_m->service_id = service_id;
    new_m->instance = instance;
    new_m->method = method;
    new_m->msg_handler = msg_handler;

    new_m->next = msg->next;
    msg->next = new_m;

    return 0;

}

msg_handler_list_t *someip_find_msg_handler(uint16_t service_id, uint16_t req_id,
        uint16_t instance, uint16_t method);
{

    msg_handler_list_t *msg = msg_headi.next;

    while(msg) {
        if(msg->service_id == service_id &&
                msg->req_id == my_id &&
                msg->instance == instance &&
                msg->method == method) {
            return msg;
        }

        msg = msg->next;
    }

    return msg;
}

int someip_del_msg_handler(msg_handler_list_t *msg_del)
{

    msg_handler_list_t *msg = &msg_head;

    while(msg) {
        if(msg->next && msg->next == msg_del) {
            msg->next = msg_del->next;
            free(msg_del);
            return 0;
        }

        msg = msg->next;
    }

    return -RTWORKS_ERRNO_NOTFOUND;
}
