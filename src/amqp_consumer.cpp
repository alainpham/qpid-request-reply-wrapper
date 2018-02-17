#include "amqp_consumer.hpp"
 
amqp_consumer::amqp_consumer(const std::string &u, const std::string &a) : url_(u), address_(a), work_queue_(0) {}

amqp_consumer::~amqp_consumer()
{
}


void amqp_consumer::on_container_start(proton::container &c)
{
    connection_ = c.connect(url_);
    
}

void amqp_consumer::on_connection_open(proton::connection &c)
{
    c.open_receiver(address_);
}

void amqp_consumer::on_message(proton::delivery &d, proton::message &m)
{
    std::cout << "#### received msg : ";
    std::cout << m.body() << std::endl;

    std::cout << "Message has reply to? " ;
    std::cout << !m.reply_to().empty() << std::endl;
    
    std::string reply_to = m.reply_to();
    proton::message reply;

    std::cout << "trying to configure reply to" << std::endl;
    reply.to(reply_to);

    std::cout << "setting body of reply" << std::endl;

    std::stringstream ss;
    ss << m.body() << " : has been processed OK";
    reply.body(ss.str()); 

    std::cout << "setting correlation id of reply";
    reply.correlation_id(m.correlation_id());
    std::cout << m.correlation_id() << std::endl;
     if (!m.reply_to().empty()){
        std::cout << "open new sender on reply_to " << reply_to << std::endl;
        proton::sender s = connection_.open_sender(reply_to); 
        std::cout << "Sending reply" << std::endl;
        s.send(reply);
        s.close();
     }

}

