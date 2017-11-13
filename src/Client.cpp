#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>
using namespace std;
void *sendThreadImp(void *arg);
void *recvThreadImp(void *arg);
int startClient(char *ip, int port);
bool online(int client, char *othername);

const char *short_options = "p:i:vh";
extern char *optarg;
const struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"ip", required_argument, NULL, 'i'},
    {"port", required_argument, NULL, 'p'},
    {"version", no_argument, NULL, 'v'},
    {NULL, no_argument, NULL, 0}};

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        cout << "Usage error. Use -h for help" << endl;
        return -1;
    }
    int opt;
    int help = 0, version = 0, port = 18086;
    char ip[20]="127.0.0.1";
    while ((opt = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1)
    {
        switch (opt)
        {
        case 'h':
            help = 1;
            break;
        case 'v':
            version = 1;
            break;
        case 'p':
            port = stoi(optarg);
            break;
        case 'i':
            strcpy(ip, optarg);
            break;
        case '?':
            cout << "未知参数" << endl;
            break;
        }
    }
    if (version == 1)
    {
        cout << "Version 1.0" << endl;
    }
    else if (help == 1)
    {
        cout << "usage:\n"
             << "\t-h, --help\t\t\tPrint this help message and exit\n"
             << "\t-i, --ip\t\t\tThe ip address you want to connect to\n"
             << "\t-p, --port\t\t\tThe port,optional,default 18086\n"
             << "\t-v, --version\t\t\tThe version\n"
             << endl;
    }
    else
    {
        startClient(ip, port);
    }
    return 0;
}
int startClient(char *ipp, int port)
{
    char ip[20];
    strcpy(ip, ipp);
    int errorCode;
    int client = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in clientaddr;
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_port = htons(port);
    clientaddr.sin_addr.s_addr = inet_addr(ip);
    errorCode = connect(client, (const struct sockaddr *)&clientaddr, sizeof(struct sockaddr));
    if (errorCode != 0)
    {
        cout << "could not connect to the Server" << endl;
    }
    else
    {
        char username[1], buff[5];
        while (true)
        {
            cout << "username(only 1 character):" << ends;
            cin >> username;
            send(client, username, strlen(username) + 1, 0);
            recv(client, buff, 5, 0);
            if (strcmp(buff, "yes") == 0)
            {
                break;
            }
            else
            {
                cout << "用户名已经存在" << endl;
            }
        }
        pthread_t recvThread, sendThread;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&recvThread, (const pthread_attr_t *)&attr, recvThreadImp, (void *)&client);
        pthread_create(&sendThread, (const pthread_attr_t *)&attr, sendThreadImp, (void *)&client);
        pthread_attr_destroy(&attr);
        pthread_join(sendThread, nullptr);
    }
    close(client);
    return 0;
}

void *recvThreadImp(void *arg)
{
    int client = *(int *)arg;
    char recvBuff[100];
    while (true)
    {
        if (0 == recv(client, recvBuff, 100, 0))
            break;
        cout << recvBuff << endl;
    }
}

void *sendThreadImp(void *arg)
{
    int client = *(int *)arg;
    char sendBuff[100];
    while (true)
    {
        cin >> sendBuff;
        send(client, sendBuff, strlen(sendBuff) + 1, 0);
        if (strcmp(sendBuff, "quit") == 0)
        {
            break;
        }
    }
    pthread_exit(nullptr);
}
