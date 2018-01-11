#include "server.h"
#include "client.h"
#include <iostream>
#include <cstring>

using namespace std;

void showHelp(){
    cout<<"Invalid arguments--\r\nUsage:\r\n\tServer : covert_channel --server [port]\r\n\t"
        <<"Client : covert_channel --client [http://[ip]:[port]] [message]";
}

int main(int argc,  char** argv) {
    if(argc < 2){
        showHelp();
        return 1;
    }

    if(argc != 2 && argc != 3 && argc != 4){
        showHelp();
        return 1;
    }

    if(!strcmp(argv[1], "--client") and !strcmp(argv[1], "--server")){
        showHelp();
        return 1;
    }

    if(!strcmp(argv[1], "--server"))
        executeServer(argc, argv);

    if(!strcmp(argv[1], "--client"))
        sendCovertMessage(argv[2], argv[3]);
}