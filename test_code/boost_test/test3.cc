#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>   //使用时间间隔  
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <functional>

// 这个例子中. 每次 定时器超时后. 都修改定时器的状态到"未超时".  再注册回调函数. 这样循环 5 次. 所以 print 会被执行 5 次.

void print(const boost::system::error_code& /*e*/,
    boost::asio::deadline_timer* t, int* count)
{
  if (*count < 5)
  {
    std::cout << *count << "\n";
    ++(*count);
    //可以用 deadline_timer::expires_at()  来 获取/设置 超时的时间点.
    //在这里我们将超时的时间点向后推迟一秒.
    t->expires_at(t->expires_at() + boost::posix_time::seconds(1));

    //再次向 t 中的 io_service 对象注册一个回掉函数.
    // 注意这里绑定时. 指定了绑定到 print 的第一个参数为: boost::asio::placeholders::error  //不懂. 这个error是什么东西. 为什么在例子2中不要绑定它?
    t->async_wait(boost::bind(print,
          boost::asio::placeholders::error, t, count));
  }
}

int main()
{
  boost::asio::io_service io;

  int count = 0;
  boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
  t.async_wait(boost::bind(print,
        boost::asio::placeholders::error, &t, &count));

  io.run();

  std::cout << "Final count is " << count << "\n";

  return 0;
}
