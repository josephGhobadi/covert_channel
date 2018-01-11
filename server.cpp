//
// Created by uoosef on 1/7/2018.
//
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>

#include <iostream>
#include "server.h"
#include "morse.h"

#define DEFAULT_PORT 12345
#define notFound -1
#define errorInFindingId "<h1>Error! i cant recognize you :(</h1>"
#define dash 1
#define dot 0
#define pangram "The quick brown fox jumps over the lazy dog";

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace std;

class MyRequestHandler : public HTTPRequestHandler {

private:
    Session initiateDBS(){
        // register SQLite connector
        SQLite::Connector::registerConnector();

        // create a session
        Session session("SQLite", "messages.db");

        Statement messages(session);
        messages << "CREATE TABLE IF NOT EXISTS messages (id INTEGER PRIMARY KEY, text VARCHAR(1024), Address VARCHAR(15))", now;
        messages.execute();

        Statement message_word(session);
        message_word << "CREATE TABLE IF NOT EXISTS message_word (id INTEGER PRIMARY KEY, message_id INTEGER, zero_one INTEGER(1))", now;
        message_word.execute();

        return session;
    }

private:
    int create_row_in_messages(string ip){
        int lastInsertedId = notFound;
        Session session = this->initiateDBS();

        Statement row(session);

        row << "INSERT INTO messages (Address) VALUES (?)", use(ip), now;

        Statement lastId(session);

        lastId << "SELECT last_insert_rowid()", into(lastInsertedId), now;
        return lastInsertedId;
    }

private:
    int extract_id_from_uri(string uri){
        int id = stoi(uri.substr (uri.rfind('/')+1));
        if(!id)
            return notFound;
        return id;
    }

private:
    void create_row_in_message_word(int id, int value){
        Session session = this->initiateDBS();

        Statement row(session);

        row << "INSERT INTO message_word (message_id, zero_one) VALUES (?, ?)", use(id), use(value), now;
    }

private:
    char join_chars_in_message_word(int id){
        Session session = this->initiateDBS();
        int zero_one=notFound;
        string binary_string="";

        Statement select(session);
        select << "SELECT zero_one FROM message_word WHERE message_id="<< id,
                into(zero_one),
                range(0, 1); //  iterate over result set one row at a time

        while (!select.done()) {
            select.execute();
            binary_string += std::to_string(zero_one);
        }
        return morseDecode(binary_string);
    }

private:
    void update_messages_text(int id, string word){
        string old="";
        Session session = this->initiateDBS();

        Statement oldText(session);

        oldText << "SELECT text FROM messages WHERE id='"<<id<<"'", into(old), now;

        old += word;

        Statement update(session);

        update << "UPDATE messages SET text='"<< old <<"' WHERE id='"<<id<<"'", now;
    }

private:
    void clear_chars_in_message_word(int id){
        Session session = this->initiateDBS();

        Statement del(session);

        del << "DELETE FROM message_word WHERE message_id='" << id << "'", now;
    }

private:
    string fetch_message_ip(int id){
        string address="";
        Session session = this->initiateDBS();

        Statement fetchIp(session);

        fetchIp << "SELECT address FROM messages WHERE id='"<<id<<"'", into(address), now;
        return address;
    }

private:
    string fetch_message_text(int id){
        string old="";
        Session session = this->initiateDBS();

        Statement oldText(session);

        oldText << "SELECT text FROM messages WHERE id='"<<id<<"'", into(old), now;
        return old;
    }

private:
    void delete_message_row(int id){
        Session session = this->initiateDBS();

        Statement del(session);

        del << "DELETE FROM messages WHERE id='" << id << "'", now;
    }

public:
    virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp) {
        int id;
        string word = "";
        resp.setStatus(HTTPResponse::HTTP_OK);
        resp.setContentType("text/html");
        ostream &out = resp.send();

        /*
         *  web server routes
         */

        if(!strcmp("/control/create", req.getURI().c_str())) {
            /*
             *  this will create url with new line and return its id
             */
            id = this->create_row_in_messages(req.getHost());
            out << id;
        }else if(req.getURI().find("/foo/") != string::npos){
            /*
             * this will create new row in message_word with message_id that
             * sent in url = {id} and zero_one = 1 or dash
             */
            id = this->extract_id_from_uri(req.getURI());
            if(id == notFound){
                out << errorInFindingId;
            }else{
                this->create_row_in_message_word(id, dash);
                out << pangram;
            }
        }else if(req.getURI().find("/bar/") != string::npos){
            /*
             * this will create new row in message_word with message_id that
             * sent in url = {id} and zero_one = 0 or dot
             */
            id = this->extract_id_from_uri(req.getURI());
            if(id == notFound){
                out << errorInFindingId;
            }else{
                this->create_row_in_message_word(id, dot);
                out << pangram;
            }
        }else if(req.getURI().find("/done/") != string::npos){
            /*
             * make a word from message char words in message_word table that have id = {id}
             * delete word's char rows in table
             * update text
             */
            id = this->extract_id_from_uri(req.getURI());
            if(id == notFound){
                out << errorInFindingId;
            }else{
                word = this->join_chars_in_message_word(id);
                this->update_messages_text(id, word);
                this->clear_chars_in_message_word(id);
                out << pangram;
            }
        }else if(req.getURI().find("/control/show/") != string::npos){
            /*
             * show message text in message and delete row
             */
            id = this->extract_id_from_uri(req.getURI());
            if(id == notFound){
                out << errorInFindingId;
            }else{
                fprintf(stderr, "\nnew Message For %s : %s", this->fetch_message_ip(id).c_str(), this->fetch_message_text(id).c_str());
                this->delete_message_row(id);
                out << pangram;
            }
        }else{
            out << "<h1>I swear I'm not a covert channel</h1>";
        }
        out.flush();
    }

private:
    static int count;
};

int MyRequestHandler::count = 0;

class MyRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
    virtual HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &) {
        return new MyRequestHandler;
    }
};

void executeServer(int argc,  char** argv) {
    uint16_t port=DEFAULT_PORT;

    if(argc == 3)
        port = (uint16_t) ~((unsigned int) atoi(argv[2]));

    HTTPServer s(new MyRequestHandlerFactory, ServerSocket(port), new HTTPServerParams);

    s.start();
    cout << endl << "Server started on : "<< port <<" TCP :) waiting for covert messages.." << endl;

    do
    {
        cout << '\n' << "Press return key to exit...";
    } while (cin.get() != '\n');

    cout << endl << "Shutting down..." << endl;
    s.stop();
}