#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>   //使用时间间隔
//一个将被定时器异步调用的函数.
void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!\n";
}

int main()
{
  boost::asio::io_service io;

  boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
  //和例子1不同. 这里调用 async_wait() 执行一个异步的等待.  它注册一个可执行体(即此处的print函数).   //这里不懂的是: print的参数怎么传入?
  //实际上. 这个执行体被注册到 deadline_timer 类的 io_service 成员上(即本例的 io 对象).  只有在以后调用 io.run() 时这些注册的执行体才会被真正执行.
  t.async_wait(print);

  //调用 io对象的 run() 函数执行那些被注册的执行体.
  //这个函数不会立即返回. 除非和他相关的定时器对象超时并且在定时器超时后执行完所有注册的执行体. 之后才返回.
  //所以它在这里阻塞一会儿. 等t超时后执行完print. 才返回.
  //这里要注意的是.  调用 io.run() 可以放在其它线程中. 那样所有的回调函数都在别的线程上运行.
  io.run();
  std::cout << "sdfasdfsdf" << std::endl;
  return 0;
}
