#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>


int ba2str(const bdaddr_t *ba, char *str)
{
        return sprintf(str, "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
                ba->b[5], ba->b[4], ba->b[3], ba->b[2], ba->b[1], ba->b[0]);
}

int main(int argc, char **argv)
{
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int ret;
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);
    struct ifreq ifr;
    int status;

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if(s == -1) {
        fprintf(stderr, "socket error %s(%d)\n", strerror(errno), errno);
        exit(2);
    }

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    ret = bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    if(ret == -1) {
        fprintf(stderr, "bind error %s(%d)\n", strerror(errno), errno);
        exit(2);
    }

    // put socket into listening mode
    ret = listen(s, 1);
    if(ret == -1) {
        fprintf(stderr, "listen error %s(%d)\n", strerror(errno), errno);
        exit(2);
    }

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    if(client == -1) {
        fprintf(stderr, "accept error %s(%d)\n", strerror(errno), errno);
        exit(2);
    }

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if(bytes_read == -1) {
        fprintf(stderr, "read error %s(%d)\n", strerror(errno), errno);
        exit(2);
    }

    printf("received %d bytes [%s]\n", bytes_read, buf);
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    fprintf(stdout, "%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    status = write(client, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), 14);

    // close connection
    close(client);
    close(s);

    return 0;
}
