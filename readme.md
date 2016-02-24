## 服务器端
servertest

## 客户端
client1

## 效果
1. 进入servertest运行make
2. 执行./main启动服务器
3. 新建一个shell做窗口1,启动./main模拟用户1在线,在client1界面执行action#register^from#1注册用户id1;
4. 新建一个shell做窗口2,启动./main模拟用户2在线,在client2界面执行action#register^from#2注册用户id2;
5. 客户进程可执行action#list，服务器会返回当前所有连接情况。
6. 客户进程可执行action#send^to#1^msg#Hello id1.向用户id1发送消息msg.

## 结果

### client1 注册
客户端成功连接服务器... 
客户机建立连接... 
action#register^from#1

### client2 注册
客户端成功连接服务器... 
客户机建立连接... 
action#register^from#2

### client2发送数据给client1

... ... 
action#send^to#1^nsg#11^msg#This is a test.
准备发送:action#send^to#1^nsg#11^msg#This is a test.
服务器回复:send success.

### client1收到client2发送过来的数据

服务器回复:This is a test.

### client1请求获取当前连接情况

action#list
准备发送:action#list

### client1获取到服务器返回的连接情况

服务器回复:(1,8),(2,7)
其中1,2表示客户id;8,7表示socket描述符

### 执行make出现错误

1. 提示/usr/bin/ld: cannot find -lxxx
ubuntu下安装apt-get install libxxx-dev
提示找不到-levent,则先安装libevent-dev

2. /home/c/.vim/tags/libevent-1.4.13-stable/下放置着libevent库
请到官网下载: http://libevent.org/

