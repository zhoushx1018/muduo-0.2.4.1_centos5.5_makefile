// Use of this source code is governed by a BSD-style license
// that can be found in the License file

// Author: superzouzhyy

// This is an internel header file, you should not include this

#ifndef MUDUO_NET_TIMERTHREAD_H
#define MUDUO_NET_TIMERTHREAD_H

#include <muduo/base/Thread.h>
#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Timestamp.h>

#include <boost/noncopyable.hpp>

namespace muduo {

namespace net {

class TimerThread : boost::noncopyable
{
 public:
  TimerThread(int timerfd);
  ~TimerThread();
  void start();
  void stop();
  void resetTimer(Timestamp const & expiration);
 
 private:
  void timerLoop();
  bool running_;
  int timerfd_;
  Timestamp expire_;
  MutexLock mutex_;
  Condition cond_;
  Thread thread_;
};

}
}
#endif // MUDUO_NET_TIMERTHREAD_H
