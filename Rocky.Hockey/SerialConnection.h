#pragma once

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/thread.hpp>
#include <cstdint>
#include <iostream>

using namespace boost::asio;

class SerialConnection
{
private:
    boost::asio::io_service m_io;
    boost::asio::serial_port m_port;

    boost::thread m_runner;
    boost::asio::streambuf m_buffer;

    bool m_quitFlag;
public:
    SerialConnection();
    ~SerialConnection();
    bool connect(const std::string& port_name, const int baud = 9600);
    void startReceive();
    void send(const std::string& text);
    void send(const uint8_t text);
    void send(const int16_t text);
    void onData(const boost::system::error_code& e, const std::size_t size);
    bool quit();
};

