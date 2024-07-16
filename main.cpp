#include "imap.hpp"

#include <iostream>
#include <stdio.h>
#include <curl/curl.h>

#if __has_include("project_defines.h")
#include "project_defines.h"
#endif

#ifndef USERNAME
#define USERNAME ""
#endif

#ifndef PASSWORD
#define PASSWORD ""
#endif

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    printf("WriteCallback\r\n");

    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(void)
{
    imap_client client;
    if(!client.authenticate(USERNAME, PASSWORD))
    {
        return -1;
    }

    std::vector<message_id_t> list;
    client.list_messages("INBOX", list);

    std::cout << "Found: " << list.size() << " messages" << std::endl;

    return 0;

    CURL *curl;
    CURLcode res = CURLE_OK;

    curl = curl_easy_init();
    if (curl)
    {
        /* Set username and password */
        curl_easy_setopt(curl, CURLOPT_USERNAME, USERNAME);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, PASSWORD);

        //curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com/INBOX/;UID=2955");
        //curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com/INBOX/;MAILINDEX=242;SECTION=TEXT");
        curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com/INBOX/");

        //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "EXAMINE INBOX");
        //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "LIST \"INBOX\" *");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SELECT INBOX");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "FETCH 1:* (FLAGS)");
        //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SEARCH NEW");
        //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SEARCH ALL");
        //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "FETCH 1:242 (BODY[HEADER.FIELDS (Subject)])");
        //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "FETCH 2 RFC822");
        //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "fetch 2 RFC822");
        //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "STATUS INBOX (UIDNEXT MESSAGES)");

        //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "FETCH 242 ALL");

        std::string readBuffer;
        //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        //curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        printf("curl_easy_setopt set\r\n");

        /* Perform the fetch */
        res = curl_easy_perform(curl);

        printf("curl_easy_perform\r\n");

        /* Check for errors */
        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        std::cout << readBuffer << std::endl;

        /* Always cleanup */
        curl_easy_cleanup(curl);
    }

    return (int)res;
}
