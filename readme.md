## 具体功能
提供用户和客服登录接口,用户发送消息给服务器,所有客服会接到抢单指令,客服根据提供的api抢单，抢单成功者将有权与用户直接通信；
为了简介起见，本服务器没有加入数据库存储功能，只提供了转发和功能实现的部分。

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

## 截图


### 执行make出现错误

1. 提示/usr/bin/ld: cannot find -lxxx
ubuntu下安装apt-get install libxxx-dev
提示找不到-levent,则先安装libevent-dev

2. /home/c/.vim/tags/libevent-1.4.13-stable/下放置着libevent库
请到官网下载: http://libevent.org/

## 具体命令请参考客户端readme以及doc目录下的协议文档
[客户端命令](client1/readme.md)

## 使用此服务器做协议的app demo请看chat-demo

    

