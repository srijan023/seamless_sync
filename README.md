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
`AI_CANONNAME` : requests a cannonical hostname  
`AI_NUMERICHOST`: specifies that hostname should be used as a numeric address string  

## sockaddr
It is also a structure with the following data elements
```c
struct sockaddr{
    unsigned short sa_family; AF_INET / AF_INET6
    char sa_data[14]; address data the size may vary depending upon the type of address
}
```

`sockaddr` is compatible with `sockaddr_in` and `sockaddr_in6` which are the *IPv4* and *IPv6* versions respectively  

## memset
It is used to allocate every single available space in memory with a single value  
```c
memset(address_start, value, size);
```

## Some Additional Funcitons
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
