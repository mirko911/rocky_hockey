#include "SerialConnection.h"



SerialConnection::SerialConnection() : m_port(m_io), m_quitFlag(false)
{
}


SerialConnection::~SerialConnection()
{
    //Stop the I/O services
    m_io.stop();
    //Wait for the thread to finish
    m_runner.join();
}

bool SerialConnection::connect(const std::string & port_name, const int baud)
{
    m_port.open(port_name);
    //Setup port
    m_port.set_option(serial_port::baud_rate(baud));
    m_port.set_option(serial_port::flow_control(
        serial_port::flow_control::none));

    if (m_port.is_open())
    {
        //Start io-service in a background thread.
        //boost::bind binds the ioservice instance
        //with the method call
        m_runner = boost::thread(
            boost::bind(
                &boost::asio::io_service::run,
                &m_io));
#ifdef __linux__
        pthread_setname_np(runner.native_handle(), "Serial Runner");
#endif
        startReceive();
    }

    return m_port.is_open();
}

void SerialConnection::startReceive()
{
    //Issue a async receive and give it a callback
    //onData that should be called when "\r\n"
    //is matched.
    async_read_until(m_port, m_buffer,
        "\r\n",
        boost::bind(&SerialConnection::onData,
            this, _1, _2));
}

void SerialConnection::send(const std::string & text)
{
    boost::asio::write(m_port, boost::asio::buffer(text));
}

void SerialConnection::send(const uint8_t text)
{
    boost::asio::write(m_port, boost::asio::buffer(&text, 1));
}

void SerialConnection::send(const int16_t text)
{
    boost::asio::write(m_port, boost::asio::buffer(&text, 1));
}


void SerialConnection::onData(const boost::system::error_code & e, const std::size_t size)
{
    if (!e)
    {
        std::istream is(&m_buffer);
        std::string data(size, '\0');
        is.read(&data[0], size);

        std::cout << "Received data:" << data << "\n";

        //If we receive quit()\r\n indicate
        //end of operations
        m_quitFlag = (data.compare("quit()\r\n") == 0);
    }

    startReceive();
}

bool SerialConnection::quit()
{
    return m_quitFlag;
}
