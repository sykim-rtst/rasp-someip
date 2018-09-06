#include <stdio.h>
#include <someip/someip.h>
#include "someip-sd.h"
#include <ethif.h>
#include <netif/etharp.h>


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

    rc1 = netconn_bind (conn, &local_ip, port);

    if (rc1 != ERR_OK) {
        printf("\r\nserver netconn_bind ERROR\r\n");

        if (conn != NULL) {
            netconn_delete(conn);
        }

        return;
    }


    while (1) {
        //	IP4_ADDR(&remote_ip, 10, 0, 0, 11);
        int err;
        struct netbuf *buf;
        u16_t len;

        if (netconn_recv (conn, &buf) == ERR_OK) {
            handle_someip_sd_packet(buf);

            netbuf_delete(buf);
        } else {
            printf ("receive fail\r\n");
        }
    }
}
void run_someip_handler(ip_addr_t *local_ip);
