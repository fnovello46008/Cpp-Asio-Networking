//
//  main.cpp
//  Networking
//
//  Created by Frank Novello on 10/3/20.
//  Copyright © 2020 Frank Novello. All rights reserved.
//

#include <iostream>

#include <chrono>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> vBuffer(20 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
    
    [&](std::error_code ec, std::size_t length)
    {
        if(!ec)
        {
            std::cout << "\n\nRead " << length << " bytes\n\n";
            
            for(int i = 0; i < length; i++)
            {
                std::cout << vBuffer[i];
            }
            
            GrabSomeData(socket);

        }
    }
                           
    );


}

int main(int argc, const char * argv[]) {

    asio::error_code ec;
    
    asio::io_context context;
    
    asio::io_context::work idleWork(context);
    
    std::thread thrContext = std::thread([&]() {
        context.run();
    });
    
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("51.38.81.49",ec),80);
    
    asio::ip::tcp::socket socket(context);
    
    socket.connect(endpoint,ec);
    
    if(!ec)
    {
        std::cout << "Connected" << std::endl;
    }else {
        std::cout << "Failed to connect \n" << ec.message() << std::endl;
    }
    
    if(socket.is_open())
    {
        GrabSomeData(socket);
        
        std::string sRequest =
        "GET /index.html HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Connection: close\r\n\r\n";
        
        socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
    
        
        
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(10000ms);
        
        context.stop();
        if(thrContext.joinable()) thrContext.join();
        
    }
    

    
    
    
    return 0;
}
