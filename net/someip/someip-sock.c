#include <stdio.h>
#include <someip/someip.h>
#include "someip-sd.h"
#include <etif.h>
#include <netif/etharp.h>


void run_someip_sd_srv(ip_addr local_ip, unsigned short port)
{
    struct netconn *conn = NULL;
    struct ip_addr ip;

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

        if ((buf = netconn_recv (conn)) != NULL) {
            handle_someip_sd_packet(buf);

            netbuf_delete(buf);
        } else {
            printf ("receive fail\r\n");
        }
    }
}

void run_someip_srv(ip_addr local_ip, unsigned short port)
{
    struct netconn *conn = NULL;
    struct ip_addr ip;

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

        if ((buf = netconn_recv (conn)) != NULL) {
            handle_someip_sd_packet(buf);

            netbuf_delete(buf);
        } else {
            printf ("receive fail\r\n");
        }
    }
}
void run_someip_handler(ip_addr local_ip);
