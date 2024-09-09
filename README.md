> [!Caution]
> This is the project under development and is not expected to work and function as per as its proposed plan.
> It is not yet cross platform and only can be compiled and executed on Linux and Mac OS

# Seamless Sync

Seamless Sync is a lightweight and efficient file transfer tool designed to seamlessly transfer files and folders between two computers over a Local Area Network (LAN). The application ensures that your files can be sent and received across multiple devices without requiring an internet connection or relying on cloud services. Seamless Sync is ideal for users who prefer to keep their data private and secure within their local network.

## Key features

- No Cloud or Database Dependency: All file transfers are done directly over the local network, ensuring privacy and security without relying on third-party cloud services or databases.
- Socket Programming: Utilizes socket programming to establish a direct and reliable connection between devices for fast and secure file transfers.
- Encryption: Ensures that your files are securely transferred across the network using AES encryption, protecting your data from unauthorized access.
- User-Friendly Interface: Minimalistic and intuitive interface that makes syncing files as easy as a few clicks.

## Why Seamless Sync?

In today's connected world, data synchronization often relies on cloud services, which may not always be desirable due to privacy concerns or lack of internet access. Seamless Sync was created to provide an alternative that keeps your data within your local network while offering the same level of convenience and reliability.

## Getting Started

At first, create a `build` dir in project folder and navigate to the newly created `build` dir.

- Run `cmake -S ..`
- Run `make`

This will create an executable in build dir..

- Execute using `bin/ssync`

> [!CAUTION]
> If the above compilation does not work due to an error regarding G*APPLICATION_DEFAULT_FLAGS, this could be due to your OS providing an older version of GLib. For GLib versions older than 2.74, you will need to replace **G_APPLICATION_DEFAULT_FLAGS** with **G_APPLICATION_FLAGS_NONE** in \_src/main.c*.

## Up coming intentions

- File change detection using double hashing technique
- File synchronization across multiple devices

## Contributing

> [!Caution]
> The project is not yet open for contributions, wait for further updates.

Seamless Sync is an open-source project, and contributions are welcome! Whether it's reporting bugs, suggesting new features, or contributing code, your help is appreciated.

## License

Seamless Sync is licensed under the MIT License.
