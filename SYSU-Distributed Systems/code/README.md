## 文件目录
-`Client.py`用户端

-`Server.py`服务器端

-`ProxyServer.py`Master端

-`ServerLaucher.py`服务器登录端设置

-`Client`用户端文件夹

-`Server`服务端文件夹

-`ProxyServer`Master端文件夹

## 运行方法

### 启动Server端

```sh
python ServerLaucher.py --n 5
```

n为服务器数量，默认为5，上限为10

### 启动Master

```sh
python ProxyServer.py --n 5
```

Master中 –n选项的值必须与ServerLaucher的值一致

### 启动Client端

```sh
python Client.py
```

