#include "../chargen/chargen.h"
#include "../daytime/daytime.h"
#include "../discard/discard.h"
#include "../echo/echo.h"
#include "../time/time.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

#include <boost/bind.hpp>

using namespace muduo;
using namespace muduo::net;

int main()
{
  LOG_INFO << "pid = " << getpid();
  EventLoop loop;

  ChargenServer ChargenServer(&loop, InetAddress(2019));
  ChargenServer.start();

  DaytimeServer daytimeServer(&loop, InetAddress(2013));
  daytimeServer.start();

  DiscardServer discardServer(&loop, InetAddress(2009));
  discardServer.start();

  EchoServer echoServer(&loop, InetAddress(2007));
  echoServer.start();

  TimeServer timeServer(&loop, InetAddress(2037));
  timeServer.start();

  loop.loop();
}

