#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

extern int v_cnt;
typedef struct { char name[32]; int value; } Var;
extern Var vars[1000];
int server_fd = -1;

void x_exec(char *line) {
    int port;
    if (sscanf(line, "WEB_LISTEN %d", &port) == 1) {
        if (server_fd != -1) return;
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in addr = {AF_INET, htons(port), INADDR_ANY};
        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        fcntl(server_fd, F_SETFL, O_NONBLOCK);
        bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
        listen(server_fd, 5);
        return;
    }

    if (server_fd != -1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

if (client_fd > 0) {
            int val = 0;
            for (int i = 0; i < v_cnt; i++) {
                if (strcmp(vars[i].name, "counter") == 0) {
                    val = vars[i].value;
                    break;
                }
            }

            char body[256];
            char out[1024];
            
            int body_len = sprintf(body, "<html><body><h1>COUNTER: %d X LANGUAGE TEST</h1></body></html>", val);
            
            int n = sprintf(out, 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %d\r\n"
                "Connection: close\r\n"
                "\r\n"
                "%s", 
                body_len, body);

            send(client_fd, out, n, 0);
            
            usleep(50000); 
            shutdown(client_fd, SHUT_WR);
            close(client_fd);
        }
	}
}