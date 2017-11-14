#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <map>
#include <set>
#include <pthread.h>
int startServer();
void *connGod(void *arg);
void *commandLine(void *arg);
std::map<char, int> users;
int main()
{
    startServer();
}

int startServer()
{
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(18086);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    bind(server, (const sockaddr *)&serveraddr, sizeof(sockaddr));
    std::cout << "Server started" << std::endl;
    listen(server, 5);
    pthread_t commandThread;
    pthread_t connThread;
    sockaddr_in clientaddr;
    unsigned len = sizeof(sockaddr_in);
    pthread_create(&connThread, nullptr, commandLine, nullptr);
    while (true)
    {
        int conn = accept(server, (sockaddr *)&clientaddr, &len);
        pthread_create(&connThread, nullptr, connGod, (void *)&conn);
    }
    pthread_join(commandThread, nullptr);
}

void *connGod(void *arg)
{
    int conn = *(int *)arg;
    char buff[100];
    char username;
    while(true){
        recv(conn, buff, 100, 0);
        username = buff[0];
        if(users.find(username)!=users.end()){
            strcpy(buff,"no");
            send(conn,buff,strlen(buff)+1,0);
        }else{
            strcpy(buff,"yes");
            send(conn,buff,strlen(buff)+1,0);
            break;
        }
    }
    users.emplace(username, conn);
    while (true)
    {
        if(0==recv(conn, buff, 100, 0))break;
        if (0 == strcmp(buff, "quit"))
        {
            break;
        }
        else if (strlen(buff) <= 3 || buff[2] == username)
        {
            strcpy(buff, "错误");
            send(conn, buff, strlen(buff) + 1, 0);
            continue;
        }
        auto othername = users.find(buff[2]);
        if (othername == users.end())
        {
            strcpy(buff, "对方未在线");
            send(conn, buff, strlen(buff) + 1, 0);
        }
        else
        {
            
            buff[0]=username;
            buff[1]='t',buff[2]='o';
            send(othername->second, buff, strlen(buff) + 1, 0);
        }
    }
    close(conn);
    users.erase(username);
    pthread_exit(nullptr);
}

void *commandLine(void *)
{
    char command[10];
    while (1)
    {
        std::cout << "ko@localhost$ " << std::ends;
        std::cin >> command;
        if (strcmp(command, "users") == 0)
        {
            for (std::pair<char, int> i : users)
            {
                std::cout << i.first << ":" << i.second << std::endl;
            }
        }
        else if (strcmp(command, "haha") == 0)
        {
            std::cout << "what did you say?" << std::endl;
        }
        else
        {
            std::cout << "unknown command" << std::endl;
        }
    }
}

