#include <stdio.h>
#include <someip/someip.h>
#include "someip-sd.h"
#include <ethif.h>
#include <netif/etharp.h>

static struct netconn *conn_someip;
ip_addr_t *myip;
unsigned short myport;


void run_someip_sd_srv(ip_addr_t *local_ip, unsigned short port)
{
    struct netconn *conn = NULL;
    ip_addr_t ip;
    int rc1;

    conn = netconn_new (NETCONN_UDP);

    if (conn == NULL) {
        /* no memory for new connection? */
        printf("\r\nserver netconn_new err\r\n");

        return;
    } else {
        printf("\r\nserver netconn_new OK\r\n");
    }

    rc1 = netconn_bind (conn, local_ip, port);

    if (rc1 != ERR_OK) {
        printf("\r\nserver netconn_bind ERROR\r\n");

        if (conn != NULL) {
            netconn_delete(conn);
        }

        return;
    }

    printf("someip-SD server ready\r\n");

    while (1) {
        //	IP4_ADDR(&remote_ip, 10, 0, 0, 11);
        int err;
        struct netbuf *buf;
        u16_t len;

        if ( netconn_recv (conn, &buf) == ERR_OK) {
            printf("connected \r\n");
            handle_someip_sd_packet(buf);

            netbuf_delete(buf);
        } else {
            printf ("receive fail\r\n");
        }
    }
}

void run_someip_srv(ip_addr_t *local_ip, unsigned short port)
{
    ip_addr_t ip;
    int rc1;
    int i;


    conn_someip = netconn_new (NETCONN_UDP);
    myip = local_ip;
    myport = port;

    if (conn_someip == NULL) {
        /* no memory for new connection? */
        printf("\r\nserver netconn_new err\r\n");

        return;
    } else {
        printf("\r\nserver netconn_new OK\r\n");
    }

    rc1 = netconn_bind (conn_someip, local_ip, port);

    if (rc1 != ERR_OK) {
        printf("\r\nserver netconn_bind ERROR\r\n");

        if (conn_someip != NULL) {
            netconn_delete(conn_someip);
        }

        return;
    }


    while (1) {
        //	IP4_ADDR(&remote_ip, 10, 0, 0, 11);
        int err;
        struct netbuf *buf;
        u16_t len;

        if (netconn_recv (conn_someip, &buf) == ERR_OK) {
            handle_someip_sd_packet(buf);

            netbuf_delete(buf);
        } else {
            printf ("receive fail\r\n");
        }
    }
}
void run_someip_handler(ip_addr_t *local_ip);

void send_packet(uint32_t ip, unsigned short port, char *packet, int len)
{
    struct netbuf *ubuf;
    char *send_data;
    ip_addr_t serverip;
    struct netconn *conn;
    int err;

    conn = netconn_new (NETCONN_UDP);
    serverip.addr = ip;

    IP4_ADDR(&serverip, 192, 168, 50, 31);
    uint8_t *iip = (uint8_t *)(&serverip);
    printf("sending ipv4 addr: %d.%d.%d.%d port: %lu\r\n", iip[0], iip[1], iip[2], iip[3], port);
    printf("\r\n(1) Sending data %d\r\n", len);

    int i;

    for(i = 0; i < len && i < 90; ++i) {
        if(i % 8 == 0) {
            printf("\r\n");
        }

        printf("%02x ", packet[i]);
    }

    err = netconn_bind (conn, myip, 12346);

    if(err != ERR_OK) {
        printf("bind err %d\r\n", err);
        return;
    } else {
        printf("connected\r\n");
    }

    err = netconn_connect (conn, &serverip, port);

    if(err != ERR_OK) {
        printf("connect err %d\r\n", err);
        return;
    } else {
        printf("connected\r\n");
    }

    ubuf = netbuf_new();
    send_data = netbuf_alloc (ubuf, len);
    memcpy(send_data, packet, len);
    printf("test\r\n");
    err = netconn_send (conn, ubuf);

    if(err != ERR_OK) {
        printf("send err %d\r\n", err);
        return;
    }

    netbuf_delete (ubuf);
    netconn_delete(conn);
}
