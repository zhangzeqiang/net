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


