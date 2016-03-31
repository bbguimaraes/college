#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

const int PORT = 8000;

int die(const char * s) {
    perror(s);
    exit(1);
}

int read_all(int fd, void * buffer, size_t count) {
    int i;
    while(count > 0) {
        if((i = read(fd, buffer, count)) <= 0)
            return 0;
        count -= i;
        buffer += i;
    }
    return 1;
}

int send_all(int fd, const void * buffer, size_t count) {
    int i;
    while(count > 0) {
        if((i = send(fd, buffer, count, 0)) <= 0)
            return 0;
        count -= i;
        buffer += i;
    }
    return 1;
}

void client(int s) {
    int other_name_len;
    if(!read_all(s, &other_name_len, sizeof(int)))
        die("read(other_name_len)");
    char other_name[other_name_len + 1];
    if(!read_all(s, &other_name, other_name_len))
        die("read(other_name)");
    other_name[other_name_len] = '\0';
    const char * name = "billy";
    const int name_len = sizeof(name);
    if(!send_all(s, &name_len, sizeof(name_len)))
        die("send(name_len)");
    if(!send_all(s, name, sizeof(name)))
        die("send(name)");
    int msg[8] = {6, 2, 5, 2, 0, 0, 0, 0};
    for(;;) {
        const int zero = 0;
        if(!send_all(s, &zero, sizeof(int)))
            die("write(Message::Movement)");
        if(!send_all(s, msg, 4 * sizeof(int)))
            die("write(msg1)");
        if(!read_all(s, msg + 4, sizeof(int)))
            die("read(Message::Movement)");
        if(msg[4] != 0)
            break;
        if(!read_all(s, msg + 4, 4 * sizeof(int)))
            die("read(msg)");
        msg[0] ^= msg[2];
        msg[2] ^= msg[0];
        msg[0] ^= msg[2];
    }
}

int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0)
        die("socket");
    struct sockaddr_in addr;
    memset((void *) &addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int ret = 1;
    if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &ret, sizeof(int)) != 0)
        die("setsockopt");
    if(bind(s, (struct sockaddr *) &addr, sizeof(addr)) !=0)
        die("bind");
    if(listen(s, 1) != 0)
        die("listen");
    unsigned int l = sizeof(addr);
    for(;;) {
        memset((void *) &addr, 0, sizeof(addr));
        int c = accept(s, (struct sockaddr *) &addr, &l);
        if(c < 0) {
            perror("accept");
            break;
        }
        if(fork()) {
            if(close(c) != 0)
                die("close");
        } else {
            if(close(s) != 0)
                die("close");
            s = c;
            client(s);
            break;
        }
    }
    wait(NULL);
    if(close(s) != 0)
        die("close");
    return 0;
}
