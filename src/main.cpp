#include <iostream>
#include <thread>

#include "amqp_consumer.hpp"

int main(int argc, char **argv)
{
    try
    {
        std::string conn_url = argc > 1 ? argv[1] : "//127.0.0.1:5672";
        std::string addr = argc > 2 ? argv[2] : "examples";
        std::cout << "starting.." << std::endl;
        amqp_consumer amqp_consumer_instance(conn_url, addr); 

        proton::container container(amqp_consumer_instance);

        std::thread container_thread([&]() { 
            std::cout << "Starting container_thread" << std::endl;
             container.run(); 

            });

        std::thread sender_thread([&]() {
            std::cout << "Starting sender_thread" << std::endl;
            // for (int i = 0; i < 10; ++i)
            // {
            //     proton::message msg(std::to_string(i + 1));
            //     std::cout << "sent \"" << msg.body() << '"' << std::endl;
            //     amqp_consumer_instance.send(msg);
            // }
        });


        sender_thread.join();
        std::cout << "sender_thread joined" << std::endl;
        // amqp_consumer_instance.close();
        container_thread.join();

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 1;
}
