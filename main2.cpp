#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <iostream>

using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
using SSLSocket = ssl::stream<tcp::socket>;

void readResponse(SSLSocket& socket, boost::asio::streambuf& response)
{
    boost::asio::read_until(socket, response, "\r\n");
    std::istream response_stream(&response);
    std::string line;
    while (std::getline(response_stream, line) && line != "\r")
    {
        std::cout << line << std::endl;
    };
}

void sendCommand(const std::string& command, SSLSocket& socket, boost::asio::streambuf& response)
{
    std::ostream request_stream(&response);
    request_stream << command;
    boost::asio::write(socket, response);
    readResponse(socket, response);
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
{
    static const std::string base64_chars = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
        
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i < 4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::string encodeOAuth2(const std::string& username, const std::string& token)
{
    std::string auth_string = "user=" + username + "\1auth=Bearer " + token + "\1\1";
    std::string encoded_auth = base64_encode(reinterpret_cast<const unsigned char*>(auth_string.c_str()), auth_string.length());
    return encoded_auth;
}

int main() {
    boost::asio::io_context context;
    ssl::context ssl_context(ssl::context::tls);

    ssl_context.set_default_verify_paths();
    //ssl_context.load_verify_file("/etc/ssl/certs/ca-bundle.crt");

    SSLSocket socket(context, ssl_context);

    tcp::resolver r(context);
    //const std::string hostname = "imap-mail.outlook.com";
    const std::string hostname = "outlook.office365.com";
    //const std::string hostname = "imap.gmail.com";
    auto endpoints = r.resolve(hostname, "993");
    boost::asio::connect(socket.next_layer(), endpoints);
    socket.set_verify_mode(ssl::verify_peer);
    socket.set_verify_callback(ssl::host_name_verification(hostname));

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if(! SSL_set_tlsext_host_name(socket.native_handle(), hostname.c_str()))
    {
        throw boost::system::system_error(
            ::ERR_get_error(), boost::asio::error::get_ssl_category());
    }

    socket.handshake(SSLSocket::client);

    //std::cout << "Attempting login" << std::endl;


    const std::string username = "";
    const std::string password = "";

    boost::asio::streambuf response;
    readResponse(socket, response);
    //sendCommand("LOGIN " + username + " " + password + "\r\n", socket, response);

    std::istream response_stream(&response);
    std::string line;
    std::getline(response_stream, line);
    //if( == "LOGIN BAD Command Error. 12") //TODO: Improve this
    {
        std::cout << "Attempting OAuth login" << std::endl;

        const std::string oauth2_token = "";
	std::string encodedOAuth = encodeOAuth2(username, oauth2_token);
        sendCommand("A001 AUTHENTICATE XOAUTH2 " + encodedOAuth + "\r\n", socket, response);
    }

    sendCommand("A002 SELECT INBOX\r\n", socket, response);

    return 0;
}
