# Http Covert Channel
An http restful covert channel based on [libpoco](https://github.com/pocoproject/poco) and [libcurl](https://github.com/curl/curl) coded with C++11 that use the [morse code](https://en.wikipedia.org/wiki/Morse_code) as character transfer protocol.
![enter image description here](http://uupload.ir/files/mgwx_cc.png)

![enter image description here](http://uupload.ir/files/1glo_morse.png)
# Usage

1- After compiling the code for the intended platform, first run an instance of the program as the server with the following command in the linux Terminal or Windows command prompt:

    ./covert_channel --server [port]
  *The default port is 12345.*

2- Now you can run another instance of the program as a client with the following command

    ./covert_channel --client [server_url] [message]
 *e.g. : ./covert_channel --client http://127.0.0.1:12345 "I Love You"*

# Compilation
At first you should download libcurl and poco c++ librarys from ther website or original repasitory of their github project

**poco** : https://pocoproject.org/download/index.html

**curl** : https://curl.haxx.se/download.html

in case of using ubuntu or cygwin you can install libcurl header files and libraries and just header files of libpoco with apt command.

*Do not forget ld command after installing new library!*

or just compile that libraries from sources with regular ./configure and make commands

After compiling libraries you can simple attach them to project with cmake configuration project file i.e. : 'CMakeLists.txt' and then start compile procedure.

i compile poco library for i386 ubuntu 16.04 that included in lib folder of project files.

# Project Structure

For simplicity i put each functionality in separate file the described below:

- main.cpp
	> To manipulate the application's execution (How should the program run? ie: client or server)

- client.cpp / client.h
	> To send the message to the server with curl.

- server.cpp / server.h
	> To set up an http server on an arbitrary tcp port and handle the sqllite database transactions using the poco library

- morse.cpp / morse.h
	> To code and decode the characters of the English language to Morse code and vice versa

## Transfer Protocol

The client can send up to 1024 characters each time including A-Z, 0-9 to the server.

*Note that the server ignore the characters case and shows them all in capital form*

The client draws each character of the message into a morse code and uses the urls in the blow table to send string to the server letter by letter.

|                URL                      |Functionality                |
|-----------------------------------------|-----------------------------|
|`/control/create`      | Indicates that the client wants to send a new message; after receiving this url by the server, it will create a new row in the message table and return that row to the client.|
|`/foo/{id}`            | After the server receives this message, it will create a new line in the message_word table that arranged with the value of its message_id equal to the id of requested url /control/create and put its zero_one 1.            |
|`/bar/{id}`            | Just like /foo/{id} but zero_one=0 |
|`/done/{id}`           | All of the zero and one related to the new character of desired message will stick together, and the result will be added to the string as the new character of related row's text field column in the messages table. |
|`/control/show/{id}`   | Prints value of text column of the row from the messages table whose id == {id}. |


***e.g. : Client wants send "HI" to server***


 1. client request new id=http://127.0.0.1:12345/control/create is 2
 2. 'H' of HI encoded to morse code = "00"
 3. client request http://127.0.0.1:12345/bar for first 0 of encoded hi H
 4. client request http://127.0.0.1:12345/bar for second 0 of encoded hi H
 5. client request http://127.0.0.1:12345/done/2 means H finished
 6. client request http://127.0.0.1:12345/bar for first 0 of encoded hi I
 7. client request http://127.0.0.1:12345/foo for second 1 of encoded hi I
 8. client request http://127.0.0.1:12345/foo for third 1 of encoded hi I
 9. client request http://127.0.0.1:12345/foo for fourth 1 of encoded hi I
 10. client request http://127.0.0.1:12345/done/2 means I finished
 11. client request http://127.0.0.1:12345/control/show/2 will show message's text in stderr of server terminal
