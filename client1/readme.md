## 客户端tcp输入详细

###  注册,class表示类型,客服-1,用户-0 

> ^SHD^{"route":"Register","from":"2", "class": 1}

> ^SHD^{"route":"Register","from":"111", "class": 0}

### 列举服务器用户客服列表() 

> ^SHD^{"route":"List","from":"111"}

### 发送消息

发送消息,class表示类型(注意不能写成字符串形式)
客服-1, 需要指定to用户id
用户-0, 不需要指定to用户id,默认从绑定关系中找目的客服id
 
> ^SHD^{"route":"Send", "class":1, "to":"111", "msg": "First send1."}

> ^SHD^{"route":"Send", "class":0, "msg": "First send1."}

### 新用户连接

通知所有客服有新用户连接

> ^SHD^{"route":"Acknowledge","to":"111", "msg": "First send."}

### 抢单

客服抢单api, action指定绑定用户id, unbind取消绑定

> ^SHD^{"route":"Bind","action":"bind","to":"111", "msg": "First send."}

> ^SHD^{"route":"Bind","action":"unbind","to":"111", "msg": "First send."}


