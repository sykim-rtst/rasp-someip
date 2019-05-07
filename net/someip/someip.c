#include <stdio.h>
#include <arpa/inet.h>
#include <someip/someip.h>
#include "ComStack_Types.h"
#include "someip-sd.h"
#include "Someip_Cfg.h"
#include "SD.h"
#include "SoAd.h"
#include "SoAd_Internal.h"


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


someip_req_t request_service(someip_app_t *app, service_t service_id, instance_t instance, method_t method,
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

void Someip_SendPacket(PduInfoType *pduInfo, SoAd_SoConIdType tx_socket)
{
    TcpIp_SockAddrType destination;
    uint8 netmask;
    TcpIp_SockAddrType default_router;
    destination.domain = TCPIP_AF_INET;
    default_router.domain = TCPIP_AF_INET;
    //(void)SoAd_GetRemoteAddr(multicast_rx_socket,  &destination);
            /* Set the remote multicast address before sending */
	destination.addr[0] = 10;
	destination.addr[1] = 10;
	destination.addr[2] = 0;
	destination.addr[3] = 22;
	destination.port = htons(SoAd_GetLastPort());
	printf("[Someip] Send Packet %d\n", htons(SoAd_GetLastPort()));
	(void)SoAd_SetRemoteAddr(tx_socket, &destination);
	 const TcpIp_SockAddrType wildcard = {
        (TcpIp_DomainType) TCPIP_AF_INET,
        TCPIP_PORT_ANY,
        {TCPIP_IPADDR_ANY, TCPIP_IPADDR_ANY, TCPIP_IPADDR_ANY, TCPIP_IPADDR_ANY }
	};

	SoAd_IfTransmit(tx_socket, pduInfo);
	(void)SoAd_SetRemoteAddr(tx_socket, &wildcard);

}

void Someip_SendRequest(someip_requested_service_t *service)
{
	uint8 buf[256];

	someip_t *data = (someip_t *)buf;
//	data->msg_id = htonl(make_message_id(service->service_id, service->instance));
	data->length = htonl(0xd);
	data->msg_id = htonl(MAKE_ID(service->service_id, service->method));
	data->req_id = htonl(MAKE_ID(service->req->client_id, service->req->req_id));
	data->protocol_ver = 0x1;
	data->interface_ver = 0x0;
	data->msg_type = 0x0;
	data->ret_code = 0x0;

	PduInfoType pduInfo;
	pduInfo.SduDataPtr = (uint8 *)data;
	pduInfo.SduLength = 8 + ntohl(data->length);
	
	strcpy(data->payload, "world");
	Someip_SendPacket(&pduInfo, 2);
}

void Someip_SendResponse(someip_requested_service_t *service, uint8 *payload, uint32 length)
{
	uint8 buf[256];
	someip_t *data = (someip_t *)buf;
	data->length = htonl(8 + length);
	data->msg_id = htonl(MAKE_ID(service->service_id, service->method));
	data->req_id = htonl(MAKE_ID(service->req->client_id, service->req->req_id));
	data->protocol_ver = 0x1;
	data->interface_ver = 0x0;
	data->msg_type = 0x80;
	data->ret_code = 0x0;

	PduInfoType pduInfo;
	pduInfo.SduDataPtr = (uint8 *)data;
	pduInfo.SduLength = 8 + ntohl(data->length);
	
	strncpy(data->payload, payload, length);
	Someip_SendPacket(&pduInfo, 1);

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
			someip_requested_service_t *service = someip_find_req_service(id, ClientId, instance);
			if(service != NULL)
				service->avail_handler(service);
			Someip_SendRequest(service);
		}
		else if(ntohl(SomeipPtr->msg_id) == RequestId)
		{
			printf("[Someip] Get Request\n");
			someip_requested_service_t *service = someip_find_req_service(id, ClientId, instance);
			if(service != NULL)
				service->avail_handler(service);

			Someip_SendResponse(service, SomeipPtr->payload, ntohl(SomeipPtr->length) - 8);
		}
		else
		{
			printf("[Someip] Unknown Services\n");
		}

	}
}




