#ifndef amqp_consumer_h
#define amqp_consumer_h

#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/tracker.hpp>
#include <proton/work_queue.hpp>
#include <proton/sender.hpp>
#include <proton/message_id.hpp>

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>

class amqp_consumer : public proton::messaging_handler
{

private:
  std::string url_;
  std::string address_;

  // Only used in proton handler thread
  proton::sender sender_;

  proton::connection connection_;

  // Shared by proton and user threads, protected by lock_
  std::mutex lock_;
  proton::work_queue *work_queue_;
  std::condition_variable sender_ready_;

public:
  amqp_consumer(const std::string &u, const std::string &a);

  ~amqp_consumer();

  //close connection
  void close();

private:
  void on_container_start(proton::container &c) override;

  void on_connection_open(proton::connection &c) override;

  void on_message(proton::delivery &d, proton::message &m) override;

};

#endif