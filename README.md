## Compiling with gtk4

At first, create a `build` dir in project folder and navigate to the newly created `build` dir.

- Run `cmake -S ..`
- Run `make`

This will create an executable in build dir..

- Execute using `bin/ssync`

> [!CAUTION]
> If the above compilation does not work due to an error regarding G_APPLICATION_DEFAULT_FLAGS, this could be due to your OS providing an older version of GLib. For GLib versions older than 2.74, you will need to replace **G_APPLICATION_DEFAULT_FLAGS** with **G_APPLICATION_FLAGS_NONE** in _src/main.c_.

# Information about the windows socket

> ⚠️ This push only contains the file supported for windows however this same could should work on unix based distros as well with the change in the header file

## addrinfo

It is a C structure with the following data members
```c
struct addrinfo{
    ai_flags = additional information for address resolution
    ai_family = address family AF_INET, AF_INET6, UNSPEC (unspecified)
    ai_socktype = SOCK_STREM, SOCK_DGRAM
    ai_protocol = 0 (any protocol automatically selected) or IPPROTO_UDP, IPPROTO_TCP
    ai_addrlen = length of address pointed by ai_addr
    ai_addr = pointer to sockaddr
    ai_canonname = pointer to canonical hostname
    ai_next = pointer to next address info in a linked list
}
```

## Common Flags

`AI_PASSIVE` : indicates socket will be used to bind
`AI_CANONNAME` : requests a canonical hostname
`AI_NUMERICHOST`: specifies that hostname should be used as a numeric address string

## sockaddr

It is also a structure with the following data elements

```c
struct sockaddr{
    unsigned short sa_family; AF_INET / AF_INET6
    char sa_data[14]; address data the size may vary depending upon the type of address
}
```

`sockaddr` is compatible with `sockaddr_in` and `sockaddr_in6` which are the _IPv4_ and _IPv6_ versions respectively

## memset

It is used to allocate every single available space in memory with a single value

```c
memset(address_start, value, size);
```

## Some Additional Functions

**htonl** : host to network long data type conversion
**htons** : host to network short data type conversion

## Socket Creation

The creation of socket requires three arguments

1. IP_Address Type
2. TCP or UDP type
3. Protocol - either TCP or UDP

## Sending information using socket

### using `sendto`:

This method is used for `UDP` socket types only and accepts the following parameters

1. socket file descriptor
2. message
3. length of message
4. optional flags
5. destination address in pointer to `sock_addr` form
6. size of destination address

This function `return` either the number of bytes sent or `-1` for failure

# Development

## The header files

### Custom Data Types Header file (`customDataTypes.h`)

This header file includes all the custom made structures for the project, which currently contains

1. `customAddInfo`:
   It includes two primitive data types, `int` and `char*` which are used mostly to return the status and message from a function.

### `getMyIp.h`

This header file is of a function named `findMyIp()` which returns the IP address of the current working device in the format of `customAddInfo`.

**Working of the function**
In this function we create a UDP Socket and connect it to the Google DNS Server and obtain the IP address of the interface used to make that connection.

**Return Value:** This function returns the message in the format of `customAddInfo` which is the **status** of function execution and the **message** with the IP address of the interface.

### `SSDPListenerConnection.h`

This is used to define a function named `SSDPListen` which is used to listen to incoming SSDP request. For this we join the SSDP request pool using the `socksetopt()` function and listen for the upcoming messages on any interface on port 1900 _(specific to SSDP requests)_

**Parameters:** This `SSDPListen()` function takes in one addition parameter which is the address of an integer, it is received by an integer pointer and is used to control the listening behavior. If the value of the integer pointed by that pointer is 0 then the listening behavior is stopped.

**Return Value:** This function returns the message in the format of `customAddInfo` which is the **status** of function execution and the **message** with the statement of how many successful SSDP requests were listened.

### `UDPErrorHandle.h`

This includes another function named `UDPHandleError()` which is used to handle the errors that can occur during the creation and working of UDP sockets.

**Parameters:** This function takes in two parameters, one is the actual message to be returned from the function specifying the error and next is the address of `customAddInfo` data type on which the message is to be written.

**Return Value:** This function does not return any sort of values just put in the message and set the status to -1 for the passed in `customAddInfo` data type.

### `UDPListen.h`
