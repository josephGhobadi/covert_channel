//
// Created by uoosef on 1/7/2018.
//
#include <iostream>
#include <cstring>
using namespace std;
#include <curl/curl.h>
#include "client.h"
#include "morse.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string fetchUrl(const char* url){
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            exit(1);
        }else{
            curl_easy_cleanup(curl);
            return readBuffer;
        }
    }
    std::cout<<"ERROR IN CURL";
    exit(1);
}

void sendChar(int id, char c, std::string req){
    std::string morse_code = morseEncode(c);
    int counter=0;
    while(morse_code.c_str()[counter]){
        if(morse_code.c_str()[counter] == '1')
            fetchUrl((req+"/foo/"+std::to_string(id)).c_str());
        else
            fetchUrl((req+"/bar/"+std::to_string(id)).c_str());
        counter++;
    }
}

void sendCovertMessage(char* url, char* message){
    std::string req = url;
    if(req.c_str()[req.length()-1] != '/')
        req += '/';
    int id = std::stoi(fetchUrl((req+"control/create").c_str()));
    int counter=0;
    while(*(message + counter)) {
        sendChar(id, *(message + counter++), req);
        fetchUrl((req+"done/"+std::to_string(id)).c_str());
    }
    fetchUrl((req+"control/show/"+std::to_string(id)).c_str());
}