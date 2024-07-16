#pragma once

#include <string>
#include <vector>

struct message_id_t
{
    bool parse(const std::string& line);
    unsigned int id;
};

class imap_client
{
public:
    imap_client();

    bool authenticate(const std::string& username, const std::string& password);
    bool list_messages(const std::string& folder, std::vector<message_id_t>& message_ids) const;

private:
    static size_t callback(void* contents, size_t size, size_t nmemb, void* userp);

    bool execute_command(const std::string& command, std::string& buffer) const;

private:
    bool authenticated;

    std::string username;
    std::string password;
};
