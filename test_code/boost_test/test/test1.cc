#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>   //使用时间间隔

int main()
{
      //所有使用 asio 的程序都必须至少拥有一个 io_service 类型的对象.
      //  //它提供 I/O 功能.
      boost::asio::io_service io;
      //
      //      //创建一个定时器 deadline_timer 的对象.
      //        //这种定时器有两种状态:  超时 和 未超时.
      //          //在超时的状态下. 调用它的 wait() 函数会立即返回. 
      //            //在未超时的情况下则阻塞. 直到变为超时状态.
      //              //它的构造函数参数为:  一个 io_service 对象(asio中主要提供IO的类都用io_service对象做构造函数第一个参数).
      //                //                     超时时间.
      //                  //从创建开始. 它就进入 "未超时"状态. 且持续指定的时间. 转变到"超时"状态.
      boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
      //
      //                      //这里等待定时器 t 超时. 所以会阻塞5秒.
      t.wait();
      //
      std::cout << "Hello, world!\n";
      //
      return 0;
}
