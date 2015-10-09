## 前言
```
此版本克隆自官方skynet，合并了sanikoyes的skynet vs2013分支版本，改动部分如下：
1、添加了skynet_win项目，默认配置成执行.\skynet .\examples\config命令行，
可以尝试在main.lua下个断点试试
2）添加了luatest项目，用来测试Babelua插件是否能够正确调试lua5.3代码
```

## 调试
```
1、安装最新版visual studio 2013扩展插件 Babelua（3.2.2.0）
2、官方提供的Babelua插件只支持lua 5.1，https://github.com/jxlczjp77/decoda.git这个修改版
支持了lua5.3和skynet调试，编译好后将LuaInject.dll替换掉Babelua插件安装目录中的对应文件
（win10在C:\Users\zjp\AppData\Local\Microsoft\VisualStudio\12.0\
Extensions\03hzxr2j.bvx\LuaInject.dll目录），或者使用预编译好的3rd/decoda/LuaInject.dll
3、由于LuaInject对多个线程调度多个lua vm处理的不好，调试时最好将skynet 
config文件的thread参数设置为1（thread = 1）

```

## 编译
```
windows：
使用visual studio 2013直接打开build/vs2013/skynet.sln即可，目前暂时只支持这一个版本的编译器

linux/macos：
官方版一样
```

## 运行
```
windows：
1、工作目录设置为skynet.exe所在目录，默认为 $(ProjectDir)..\..\
2、命令参数设置为config文件的相对路径，如 examples/config

linux/macos：
和官方版一样
```

## Build

For windows, open build/vs2013/skynet.sln and build all
You can use vs ide to debugging skynet

```
## Difference between offical skynet
1.sproto support real(double)/variant(real/int/string) field type
2.used event-select to simulate epoll
3.use socket api to simulate pipe()
4.hack read fd(0) for console input
```

For linux, install autoconf first for jemalloc

```
git clone https://github.com/cloudwu/skynet.git
cd skynet
make 'PLATFORM'  # PLATFORM can be linux, macosx, freebsd now
```

Or you can :

```
export PLAT=linux
make
```

For freeBSD , use gmake instead of make .

## Test

Run these in different console

```
./skynet examples/config	# Launch first skynet node  (Gate server) and a skynet-master (see config for standalone option)
./3rd/lua/lua examples/client.lua 	# Launch a client, and try to input hello.
```

## About Lua

Skynet now use a modify version of lua 5.3.1 (http://www.lua.org/ftp/lua-5.3.1.tar.gz) .

For detail : http://lua-users.org/lists/lua-l/2014-03/msg00489.html

You can also use the other official Lua version , edit the makefile by yourself .

## How To (in Chinese)

* Read Wiki https://github.com/cloudwu/skynet/wiki
* The FAQ in wiki https://github.com/cloudwu/skynet/wiki/FAQ
