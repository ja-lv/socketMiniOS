# Socket Mini OS
C++ client-server program that mimics an operating system's disk and file system.

## Requirements
Linux OS
C++ compiler

## Install
Run the following on shell command from the project's root folder:

`make all`

## Run
From the build folder run:

### -Server/ 

`./basic_server`

The server will throw the **hostname** and **port number** clients will require.
It listens for the following commands where str is a string.

* `reverse str` - Reverses a string
* `ls` - Returns the output from ls command. *****

`./disk_server c s filename` where c is cylinder sizer, and s is sector size.

The server will throw the **hostname** and **port number** clients will require. Block size is set to 128 bytes.
It listens for the following commands from a client.

* `I` - Returns c s
* `R c s` - Returns a message up to 128 bytes at disk location c s.
* `W c s l data` - Writes a message, data with l bytes, on the disk. Up to 128 bytes at disk location c s.
* `exit` - disconnects from the client and exits the program.

`./filesystem_server hostname port` where hostname and port are required to connect to a disk_server.

* `F` - Formats the disk and directory.
* `C f` - Creates a file with the filename f.
* `D f` - Deles a file with filename f.
* `L b` - Lists a directory, with arguments b of 0 or 1. 1 provides more information, 0 just file names.
* `R f` - Reads what is stored in filename f.
* `W f l data` - Writes into filename f up to 128 bytes of data. l bytes of data is written.
* `mkdir dirname` - creates a new directory.
* `cd dirname` - changes directory to dirname.
* `pwd` - prints the current directory.
* `rmdir dirname` - deletes the given dirname directory.
* `exit` - disconnects from the client and exits the program.

**FILE SYSTEM SERVER REQUIRES A CONNECTION TO A DISK SERVER**

The server will throw the **hostname** and **port number** clients will require. Block size is set to 128 bytes.
It listens for the following commands from a client.


### -Client/

`./basic_client`

The client will ask for hostname and port to connect with a server. 
It will then ask for a message to send, and display messages the server returns
Basic client can connect and send messages to all of the servers above.

Further commands -
* `exit` - disconnects from the server and exits the program.
