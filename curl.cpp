/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * SPDX-License-Identifier: curl
 *
 ***************************************************************************/
 
/* <DESC>
 * Retrieve IMAP emails
 * </DESC>
 */
 
#include <iostream>
#include <stdio.h>
#include <curl/curl.h>
 
/* This is a simple example showing how to fetch mail using libcurl's IMAP
 * capabilities.
 *
 * Note that this example requires libcurl 7.30.0 or above.
 */

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    printf("WriteCallback\r\n");

    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(void)
{
  CURL *curl;
  CURLcode res = CURLE_OK;
 
  curl = curl_easy_init();
  if(curl) {
    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, "");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "");
 
    //curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com/INBOX/;UID=2955");
    curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com/INBOX/;MAILINDEX=242;SECTION=TEXT");
    //curl_easy_setopt(curl, CURLOPT_URL, "imaps://imap.gmail.com/INBOX/");

    //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "EXAMINE INBOX");
    //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SEARCH NEW");
    //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SEARCH ALL");
    //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "FETCH 1:242 (BODY[HEADER.FIELDS (Subject)])");
    //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "FETCH 2 RFC822");
    //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "fetch 2 RFC822");
    //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "STATUS INBOX (UIDNEXT MESSAGES)");

    //curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "FETCH 242 ALL");

    std::string readBuffer;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    printf("curl_easy_setopt set\r\n");
 
    /* Perform the fetch */
    res = curl_easy_perform(curl);

    printf("curl_easy_perform\r\n");
 
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    std::cout << readBuffer << std::endl;
 
    /* Always cleanup */
    curl_easy_cleanup(curl);
  }
 
  return (int)res;
}
