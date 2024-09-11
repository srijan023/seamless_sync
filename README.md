# Seamless Sync

**Seamless Sync** is a lightweight and efficient **_file transfer and chat tool_** designed to seamlessly exchange files and messages between two computers over a Local Area Network (LAN). The application ensures that files can be sent and received securely across multiple devices without requiring an internet connection or relying on cloud services. Seamless Sync is ideal for users who prioritize privacy and security within their local network.

## Why Seamless Sync?

In today's connected world, data synchronization often relies on cloud services, which may raise concerns around privacy or be unavailable due to lack of internet access. **Seamless Sync** provides a **secure and direct local network solution**, giving users control over their data while offering convenience, reliability and simplicity.

## Key features

- **Privacy and Security**: All file transfers and messages are sent directly over the local network, ensuring data is kept private without third-party involvement.

- **Socket Programming**: Uses socket programming for reliable communication between devices, enabling fast and secure file transfers.

- **Encrypted Transfers**: Files are securely transferred using AES encryption, protecting data from unauthorized access during the transfer.

- **Chat Integration**: Allows for real-time messaging between connected devices along with file transfer.

- **User-Friendly Interface**: Provides a simple and intuitive interface built using GTK, making file transfers as easy as a few clicks.

## Dependencies

To compile and run **Seamless Sync**, the following dependencies are required:

### 1. GTK4:

- Used to create the graphical user interface(GUI). Ensure you have GTK4 installed on your system.

- Install on Ubuntu/Debian-based systems:

```bash
sudo apt-get install libgtk-4-dev
```

- Install on Arch:

```bash
sudo pacman -Syu gtk4
```

### 2. GLib:

- Glib is required for various utilities like data structures, event loops, and GApplication.

- Ensure GLib version 2.74 or higher is installed. For older versions, modify the code as [mentioned below](#known-issues).

- GLib is automatically installed when you install **GTK4**.

### 3. CMake:

- Used for project configuration and building.

- Install on Ubuntu/Debian-based systems:

```bash
sudo apt-get install cmake
```

- Install on Arch:

```bash
sudo pacman -Syu cmake
```

### 4. GCC or Clang:

- A C compiler to compile the project.

### 5. Sockets Library:

- Required for socket-based communication between the server and client.

- Should be part of your system's default C libraries.

## Compilation Instructions

### Step 1: Clone the Repository

```bash
git clone https://github.com/srijan023/seamless_sync.git
cd seamless_sync
```

### Step 2: Create a Build Directory

In the **project directory**, create a `build` directory:

```bash
mkdir build
cd build
```

### Step 3: Run CMake

Generate the makefiles using CMake:

```bash
cmake -S ..
```

### Step 4: Compile the Project

Once CMake has generated the makefiles, compile the project:

```bash
make
```

### Step 5: Run the Application

After successful compilation, run the executable:

```bash
bin/ssync
```

## Known Issues

<!-- prettier-ignore -->
> [!IMPORTANT] 
> **GLib Version Compatibility**: If you encounter issues related to `G_APPLICATION_DEFAULT_FLAGS` during compilation, it could be due to using an older version of GLib (earlier than 2.74). In that case, replace `G_APPLICATION_DEFAULT_FLAGS` with `G_APPLICATION_FLAGS_NONE` in `src/main.c` to resolve the issue.

## Upcoming Features

- **File Change Detection**: Implementing file synchronization with change detection using a double hashing technique, allowing for efficient updates of modified files.

- **Multi-device Synchronization**: Future updates will allow file synchronization across multiple devices simultaneously.

## Contributing

> [!NOTE]
> The project is still under active development and not yet open for external contributions. Stay tuned for future updates.

Once **Seamless Sync** is stable, contributions will be welcome! Whether you want to report bugs, suggest new features, or contribute code, your input will be appreciated.

### How to Contribute

1. Fork the repository.

2. Create a new branch for your feature or bug fix.

3. Submit a pull request once you have tested your changes.

## License

Seamless Sync is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

# Disclaimer

<!-- prettier-ignore -->
> [!WARNING]
> This project is under active development. Some features may not function as expected, and the project may contain bugs or unfinished code. Seamless Sync is currently compatible only with Linux and macOS. Windows support may be added in the future.
