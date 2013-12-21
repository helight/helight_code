= Simple-TODO Bottle 实现版 =

Changelog::
    - 110703 增补完成文档,向社区发布...
    - 110701 在伟大的党的90周年生日之时，为了庆贺这一伟大的时刻，创建工程

== 目标 ==
展示 Bottle 框架的方便，分享 KISS 原则的web 应用开发经验

== 使用 ==
确信:
# Python > 2.5.*
# Hg > 1.8.*

部署和启动:

**以Ubuntu 环境为例,其它OS 情况类似**
{{{
$ mkdir -p /path/2/u/workspace/
$ hg clone https://bitbucket.org/ZoomQuiet/bottle-simple-todo
$ cd bottle-simple-todo
$ python zqbtodo.py
Bottle server starting up (using WSGIRefServer())...
Listening on http://0.0.0.0:9126/
Use Ctrl-C to quit.
...
}}}
使用任意浏览器访问: **http://127.0.0.1:9126**
