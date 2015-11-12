#include <stdio.h>
#include <string.h>
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


int bachk(const char *str)
{
        if (!str)
                return -1;

        if (strlen(str) != 17)
                return -1;

        while (*str) {
                if (!isxdigit(*str++))
                        return -1;

                if (!isxdigit(*str++))
                        return -1;

                if (*str == 0)
                        break;

                if (*str++ != ':')
                        return -1;
        }

        return 0;
}

int str2ba(const char *str, bdaddr_t *ba)
{
        int i;

        if (bachk(str) < 0) {
                memset(ba, 0, sizeof(*ba));
                return -1;
        }

        for (i = 5; i >= 0; i--, str += 3)
                ba->b[i] = strtol(str, NULL, 16);

        return 0;
}

int main(int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[64];
    char buf[128];
    int fd;
    struct ifreq ifr;
    int count = 1;
    char bd_addr[10][64];
    int id;
    char cmd[128];
    char rssi[64];
    FILE *fp1;

    system("hcitool scan > /tmp/hciinfo");
    FILE *fp = fopen("/tmp/hciinfo", "r");
    fgets(buf, sizeof(buf), fp);
    fprintf(stdout, "\nID   Bluetooth Address       RSSI\n");
    while (fgets(buf, sizeof(buf), fp) != 0) {
    /*...*/
        strncpy(bd_addr[count], &buf[1], 17);
        bd_addr[count][17]='\0';
        sprintf(cmd, "hcitool cc %s", bd_addr[count]);
        cmd[28]='\0';
        system(cmd);
        sprintf(cmd, "hcitool rssi %s > /tmp/rssi", bd_addr[count]);
        cmd[42]='\0';
        system(cmd);
        cmd[0]='\0';
        fp1 = fopen("/tmp/rssi", "r");
        fgets(cmd, sizeof(cmd), fp1);
        fclose(fp1);
        strncpy(rssi, &cmd[19], 3);
        cmd[0]='\0';
        fprintf(stdout, "\n%d    %s     %s", count, bd_addr[count], rssi);
	count++;
    }
    fclose(fp);
    fprintf(stdout, "\nEnter the codec to be paired with\n");
    scanf("%d", &id);


    if(bd_addr[id] == NULL) {
        fprintf(stderr,"Usage: %s <Server Address>\n", argv[0]);
        exit(2);
    }
    strcpy(dest, bd_addr[id]);
    fprintf(stdout, "Dest = %s\n", dest);

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if(s == -1) {
        fprintf(stderr, "socket error %s(%d)\n", strerror(errno), errno);
        exit(2);
    }

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if(status == -1) {
        fprintf(stderr, "connect error %s(%d)\n", strerror(errno), errno);
        exit(2);
    }
    fprintf(stdout, "connect done\n");
 
    sleep(5);
    fprintf(stdout, "Writing data\n");


    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    fprintf(stdout, "%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    status = write(s, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr), 14);

    close(s);

    return 0;
}
