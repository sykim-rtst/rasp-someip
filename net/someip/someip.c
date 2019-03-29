#include <stdio.h>
#include <someip/someip.h>
#include "ComStack_Types.h"
#include "someip-sd.h"
#include "Someip_Cfg.h"

#define MAKE_ID(x, y) (((uint32)(x) << 16) | (y))

someip_app_t *someip_register_app(client_t my_id)
{
    someip_app_t *srv;

    srv = (someip_app_t *)malloc(sizeof(someip_app_t));

    if(!srv) {
        return NULL;
    }

    srv->client_id = my_id;
    return srv;
}

int someip_register_msg_handler(client_t my_id, service_t service_id,
                                instance_t instance, method_t method, void (*msg_handler)(someip_t *someip))
{
    int err;

    err = someip_add_msg_handler(my_id, service_id, instance, method, msg_handler);

    return err;
}

int someip_unregister_msg_handler(service_t my_id, service_t service_id,
                                  instance_t instance, method_t method)
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

int someip_send_request(service_t service, instance_t instance, someip_t *packet)
{
    someip_offering_service_t *offer =
        someip_find_offering_service(service, instance);

    if(!offer) {
        return -1;
    }


    send_packet(offer->ipv4_addr, offer->port, (char *)packet, packet->length + 8);

}

/*
int someip_register_state_handler(service_t my_id, service_t service_id,
                                  void (*state_hander)(int state));
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

*/

someip_req_t request_service(someip_app_t *app, service_t service_id, instance_t instance,
                             void (*avail_handler)(service_t service, instance_t instance, int available) )
{
    someip_requested_service_t *srv;

    srv = (someip_requested_service_t *)malloc(sizeof(someip_requested_service_t));

    if(!srv) {
        return NULL;
    }

    srv->req = app;
    srv->service_id = service_id;
    srv->instance = instance;
    srv->avail_handler = avail_handler;

    someip_add_req_service(srv);


    return (someip_req_t)srv;

}

//int release_service(service_t my_id, service_t service_id, instance_t instance);

static someip_app_t *app;

void sample_avail_handler(someip_requested_service_t *service)
{
}

void sample_msg_handler(someip_t *someip)
{
}



static uint16_t MethodId = 0x3333;
static uint16_t ClientId = 0x4444;

void Someip_Init()
{
	int i;
	printf("[Someip] Someip_Init \n");
	app = someip_register_app(ClientId);
	for(i=0; i < Someip_Instance.NoOfServices; i++)
	{
		service_t service_id = (service_t)Someip_Instance.Service[i].Id;
		instance_t instance = (instance_t)Someip_Instance.Service[i].InstanceId;
		request_service(app, service_id, instance, MethodId, sample_avail_handler);
	}
}

void Someip_MainFunction()
{
}

void Someip_RxIndication(PduIdType RxPduId, const PduInfoType *PduData)
{
	uint8 *data = PduData->SduDataPtr;
	someip_t *SomeipPtr = (someip_t *)data;
	int i;
	uint32 RequestId, NotifyId;
	printf("[Someip] Someip_RxIndication %d\n", RxPduId);
	for(i=0; i < Someip_Instance.NoOfServices; i++)
	{
		service_t id = Someip_Instance.Service[i].Id;
		instance_t instance = Someip_Instance.Service[i].InstanceId;
		method_t method = Someip_Instance.Service[i].MethodId;
		uint16_t event = Someip_Instance.Service[i].EventId;
		NotifyId = MAKE_ID(id, event);
		RequestId = MAKE_ID(id, method);
		printf("%x %x %x\n", SomeipPtr->msg_id, NotifyId, RequestId);
		if(ntohl(SomeipPtr->msg_id) == NotifyId)
		{
			printf("[Someip] Get Notification\n");
		}
		else if(ntohl(SomeipPtr->msg_id) == RequestId)
		{
			printf("[Someip] Get Request\n");
		}
		else
		{
			printf("[Someip] Unknown Services\n");
		}

	}
}




