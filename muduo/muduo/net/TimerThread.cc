// Use of this source code is gnoverned by a BSD-style license
// that can be found in the License file

#include <muduo/net/TimerThread.h>
#include <muduo/net/SocketsOps.h>
#include <muduo/base/Logging.h>

#include <boost/bind.hpp>

namespace muduo
{
namespace net
{
namespace detail
{
  
void writeTimerfd(int timerfd, Timestamp now)
{
  uint64_t howmany = 1;
  ssize_t n = ::write(timerfd, &howmany, sizeof howmany);
  LOG_TRACE << "TimerThread::timeLoop writes" << howmany << " at " << now.toString();
  if (n != sizeof howmany)
  {
    LOG_ERROR << "TiemerThread::timerLoop writes " << n << " bytes instead of 8";
  }
}

}
}
}

using namespace muduo;
using namespace muduo::net;
using namespace muduo::net::detail;

TimerThread::TimerThread(int timerfd)
  : running_(false),
    timerfd_(timerfd),
    expire_(0),
    mutex_(),
    cond_(mutex_),
    thread_(boost::bind(&TimerThread::timerLoop, this))
{
}

TimerThread::~TimerThread()
{
  stop();
}

void TimerThread::stop()
{
  if (running_) 
  {
    running_ = false;
    cond_.notify();
    thread_.join();
  }
}

void TimerThread::start()
{
  assert(!thread_.started());
  running_ = true;
  thread_.start();
}

void TimerThread::resetTimer(Timestamp const & expiration)
{
  MutexLockGuard lock(mutex_);
  expire_ = expiration;
  cond_.notify();
}

void TimerThread::timerLoop()
{
  while (running_) 
  {
    MutexLockGuard lock(mutex_);
    Timestamp that(0);
    while (!expire_.valid() && running_) 
    {
      cond_.wait();
    }

    if (expire_ > Timestamp::now()  && running_)
    {
      if (!cond_.timewait(expire_)) // timeout
      { 
        writeTimerfd(timerfd_, Timestamp::now());
        expire_.swap(that);
      }
    }
    else if (expire_ <= Timestamp::now())
    {
      writeTimerfd(timerfd_, Timestamp::now());
      expire_.swap(that);
    }
  }

}
