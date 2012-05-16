// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)
// Modify: Add cond timewait 

#ifndef MUDUO_BASE_CONDITION_H
#define MUDUO_BASE_CONDITION_H

#include <muduo/base/Mutex.h>
#include <muduo/base/Timestamp.h>

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace muduo
{

class Condition : boost::noncopyable
{
 public:
  explicit Condition(MutexLock& mutex) : mutex_(mutex)
  {
    pthread_cond_init(&pcond_, NULL);
  }

  ~Condition()
  {
    pthread_cond_destroy(&pcond_);
  }

  void wait()
  {
    pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
  }

  // rc == ETIMEOUT return false, other error ignore
  bool timewait(Timestamp const & timeout)
  {
    struct timespec ts =  timeout.timespceSinceEpoch();
    int rc = pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &ts);
    return (rc == 0 ? true : false);
  }

  void notify()
  {
    pthread_cond_signal(&pcond_);
  }

  void notifyAll()
  {
    pthread_cond_broadcast(&pcond_);
  }

 private:
  MutexLock& mutex_;
  pthread_cond_t pcond_;
};

}
#endif  // MUDUO_BASE_CONDITION_H
