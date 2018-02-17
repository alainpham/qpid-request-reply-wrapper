#include "amqp_producer.hpp"

amqp_producer::amqp_producer(const std::string &u, const std::string &a) : url_(u), address_(a), work_queue_(0) {}  

amqp_producer::~amqp_producer()
{
}

// Thread safe
void amqp_producer::close()
{
    work_queue()->add([=]() { sender_.connection().close(); });
}

// Thread safe
void amqp_producer::send(const proton::message &msg)
{
    // Use [=] to copy the message, we cannot pass it by reference since it
    // will be used in another thread.
    std::cout << "Trying to send msg" << std::endl;
    work_queue()->add([=]() { sender_.send(msg); });
}

proton::work_queue *amqp_producer::work_queue()
{
    // Wait till work_queue_ and sender_ are initialized.
    std::unique_lock<std::mutex> l(lock_);
    std::cout << "work_queue lock" + (!work_queue_) << std::endl;
    while (!work_queue_)
    {
        std::cout << "waiting for sender to get ready" << std::endl;
        sender_ready_.wait(l);
        std::cout << "Workqueue ready" << std::endl;
    }

    return work_queue_;
}

void amqp_producer::on_container_start(proton::container &c)
{
    connection_ = c.connect(url_);
    
}

void amqp_producer::on_connection_open(proton::connection &c)
{
    c.open_sender(address_);
    proton::receiver r = c.open_receiver(address_);
}

void amqp_producer::on_message(proton::delivery &d, proton::message &m)
{
    std::cout << "#### received msg : ";
    std::cout << m.body() << std::endl;

}

void amqp_producer::on_sender_open(proton::sender &s)
{
    // sender_ and work_queue_ must be set atomically
    std::lock_guard<std::mutex> l(lock_);
    std::cout << "on_sender_open lock_guard" << std::endl;
    sender_ = s;
    work_queue_ = &s.work_queue();
    sender_ready_.notify_all();
    std::cout << "SenderReady" << std::endl;
}
