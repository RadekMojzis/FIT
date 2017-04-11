#ftrest(1)/ftrestd(1)


##NAME
Client/Server application using BSD sockets, HTTP protocol and REST API.

##SYNOPSIS
**ftrest** COMMAND REMOTE-PATH [LOCAL-PATH]

**ftrestd** [**-r** ROOT-FOLDER] [**-p** PORT]

##DESCRIPTION
**ftrest** is a client part of a simple network application. It sends requests in form 
of HTTP requests to **ftrestd**, the server part, which interprets the HTTP request and sends a response. 
Together they work as a simple network file manager.

##OPTIONS

**COMMAND**

- **del** Deletes a file on the server specified by **REMOTE-PATH**.
- **get** Copies a file specified by **REMOTE-PATH** into current working directory or to **LOCAL-PATH** if specified.
- **put** Copies a file specified in **LOCAL-PATH** into **REMOTE-PATH** on the server.
- **lst** Lists the content of **REMOTE-PATH**.
- **mkd** Creates a directory specified in **REMOTE-PATH** on the server.
- **rmd** Removes a directory specified in **REMOTE-PATH** from the server.

**REMOTE-PATH**

- Specifies path to a remote file or directory

**LOCAL-PATH**

- Specifies a path to a local file or directory

**[-p PORT]**

- Specifies a port the server will be listening to. by **default** it will be listening on port **6677**.

**[-r ROOT-FOLDER]**

- Specifies a **ROOT-FOLDER** on the server, by default the **ROOT-FOLDER** is folder from which the server is being ran.

##DIAGNOSTICS
- **"Not a directory."** When **REMOTE-PATH** specifies a file but the command is **lst** or **rmd**.
- **"Directory not found."** When **REMOTE-PATH** does not specify any existing object while using **lst** or **rmd**.
- **"Directory not empty."** When **REMOTE-PATH** specifies a directory which is not empty while using **rmdir**.
- **"Already exists."** When **REMOTE-PATH** specifies a folder/file that already exists while using **mkd** or **put**.
- **"Not a file."** When **REMOTE-PATH** specifies a directory while using **del** or **get**.
- **"File not found."** When **REMOTE-PATH** does not specify any object while using **del** or **get**.
- **"Unknown error."** for other errors.

##AUTHOR
**Radek Mojžíš**, 
	**xmojzi07@stud.fit.vutbr.cz**