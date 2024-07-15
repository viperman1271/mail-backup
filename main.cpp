#include <mailio/message.hpp>
#include <mailio/imap.hpp>
#include <iostream>
#include <fstream>

void fetchEmails(const std::string& server, const std::string& username, const std::string& password) 
{
    try 
    {
        mailio::imaps conn(server, 993);

	mailio::dialog_ssl::ssl_options_t options;
        options.verify_mode = boost::asio::ssl::verify_none;
	conn.ssl_options(options);

        std::cout << "Authenticating" << std::endl;
	conn.authenticate(username, password, mailio::imaps::auth_method_t::LOGIN);
	mailio::message msg;

        std::cout << "Gathering mailbox information" << std::endl;
        mailio::imaps::mailbox_stat_t stat = conn.select("INBOX");
        const int mail_count = stat.messages_no;

	for (int i = 1; i <= mail_count; ++i) 
	{
            conn.fetch(i, msg);
            std::string filename = "email_" + std::to_string(i) + ".eml";
	    //std::ofstream outFile(filename);
            //outFile << msg;
	    //outFile.close();
        }
    } 
    catch (const mailio::imap_error& e) 
    {
        std::cerr << "IMAP error: " << e.what() << std::endl;
    } 
    catch (const mailio::dialog_error& e) 
    {
        std::cerr << "Dialog error: " << e.what() << std::endl;
    }
    catch (const boost::system::system_error& e)
    {
        std::cerr << "Boost error: " << e.what() << std::endl;
    }
}

int main()
{
    //const std::string server = "imap.your-email-provider.com"; 
    const std::string server = "";
    const std::string username = "";
    const std::string password = "";

    fetchEmails(server, username, password);

    return 0;
}
