#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

  #define RESET   "\033[0m"
  #define RED     "\033[31m"      /* Red */
  #define GREEN   "\033[32m"      /* Green */

    struct hostent *host;
    int err, i, sock;
    char* hostname;
    struct sockaddr_in sa;

    if(argv[1]) {
      hostname = argv[1];
    } else {
      printf(RED "Host Name is not defined\n" RESET);
      exit(2);
    }

    int ports[27] = {21, 22, 23, 25, 26, 2525, 53, 80, 110, 119, 123, 143,
                     161, 194, 443, 587, 993, 995, 2077, 2078, 2082, 2083,
                     2086, 2087, 2095, 2096, 3306};
    /*
      21: File Transfer Protocol (FTP)
      22: Secure Shell (SSH)
      23: Telnet remote login service
      25 (Alternates: Port 26 / Port 2525): Simple Mail Transfer Protocol (SMTP)
      53: Domain Name System (DNS) service
      80: Hypertext Transfer Protocol (HTTP) used in the World Wide Web
      110: Post Office Protocol (POP3)
      119: Network News Transfer Protocol (NNTP)
      123: Network Time Protocol (NTP)
      143: Internet Message Access Protocol (IMAP)
      161: Simple Network Management Protocol (SNMP)
      194: Internet Relay Chat (IRC)
      443: HTTP Secure (HTTPS)
      995: POP3 SSL
      993: IMAP SSL
      587: SMTP SSL
      3306: MySQL
      2082: CPanel
      2083: CPanel SSL
      2086: WHM (Webhost Manager)
      2087: WHM (Webhost Manager) SSL
      2095: Webmail
      2096: Webmail SSL
      2077: WebDAV/WebDisk
      2078: WebDAV/WebDisk SSL
    */
    printf("\n");

    strncpy((char*)&sa, "", sizeof sa);
    sa.sin_family = AF_INET;

    if(isdigit(hostname[0])){
        sa.sin_addr.s_addr = inet_addr(hostname);
    }
    else if (  (host = gethostbyname(hostname)) != 0){
        strncpy((char*)&sa.sin_addr, (char*) host->h_addr, sizeof sa.sin_addr);
    }
    else {
        herror(hostname);
        exit(2);
    }

    for(i = 0; i < sizeof(ports)/sizeof(ports[0]); i++) {
        printf("Scanning port %i\n",ports[i]);

        sa.sin_port = htons(ports[i]);
        sock = socket(AF_INET, SOCK_STREAM, 0);

        if(socket < 0){
            exit(1);
        }

        err = connect(sock, (struct sockaddr*)&sa, sizeof sa);

        if (err < 0){
            fflush(stdout);
        }
        else {
            printf(GREEN "Port %i is open\n" RESET, ports[i]);
        }
        close(sock);
    }
    fflush(stdout);
    return 0;
}
