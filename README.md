# TermChat
在终端网络聊天程序
## 启动服务端 ##
使用<code>./Server.out</code>启动服务器端
## 启动客户端 ##
使用<code>./Client.out</code>启动程序
> * -h, --help 
>>  打印帮助
>  * -i, --ip 
>>指定需要连接的服务器IP,默认连接本地127.0.0.1
 > * -p, --port        
 >>指定端口，默认18086
 > * -v, --version     
 >>打印版本信息
 
## 客户端操作 ##

> * <code>quit</code>
>> 不再需要发送消息，退出客户端
> * <code>toaMessage</code>
>>给用户a发送消息Message，
## 服务端操作 ##
>* <code>users</code>
>> 以"用户名:socket描述符"的方式显示当前所有在线用户
