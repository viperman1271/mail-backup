#include "imap.hpp"

#include <curl/curl.h>

#include <iostream>
#include <sstream>

const std::string TOKEN_SEPARATOR_STR{" "};

bool message_id_t::parse(const std::string& line)
{
    std::string::size_type tag_pos = line.find(TOKEN_SEPARATOR_STR);
    if (tag_pos == std::string::npos)
    {
        return false;
    }

    const std::string tag = line.substr(0, tag_pos);

    const std::string::size_type result_pos = line.find(TOKEN_SEPARATOR_STR, tag_pos + 1);
    const std::string result = line.substr(tag_pos + 1, result_pos - tag_pos - 1);

    //const std::string response = line.substr(tag_pos + 1);

    try
    {
        std::size_t pos;
        id = std::stoi(result, &pos);
    }
    catch (std::invalid_argument const& e)
    {
        return false;
    }
    catch (std::out_of_range const& ex)
    {
        return false;
    }

    return true;
}

imap_client::imap_client()
: authenticated(false)
{
}

bool imap_client::authenticate(const std::string& username, const std::string& password)
{
    if (CURL* const curl = curl_easy_init())
    {
        curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com/");

        curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "NOOP");

        std::string readBuffer;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, imap_client::callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        const CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return false;
        }

        this->username = username;
        this->password = password;

        authenticated = true;

        return true;
    }

    std::cerr << "curl_easy_init() failed" << std::endl;

    return false;
}

bool imap_client::list_messages(const std::string& folder, std::vector<message_id_t>& message_ids) const
{
    if(!authenticated)
    {
        return false;
    }

    std::string buffer;
    if(execute_command("FETCH 1:* (FLAGS)", buffer))
    {
        std::stringstream ss { buffer };
        for (std::string line; std::getline(ss, line);)
        {
            message_id_t message_id;
            if(message_id.parse(line))
            {
                message_ids.push_back(message_id);
            }
        }

        return true;
    }

    return false;
}

size_t imap_client::callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool imap_client::execute_command(const std::string& command, std::string& buffer) const
{
    if (CURL* const curl = curl_easy_init())
    {
        curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com/INBOX/"); //TODO: Allow for specifying a folder

        curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, command.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, imap_client::callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

        const CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return false;
        }

        return true;
    }

    return false;
}
