# Real-Time Chat Application in C++  

## **📌 Overview**
This project is a **real-time client-server chat application** built using **C++**, leveraging **Winsock and Boost.Asio** for networking and **MongoDB** for secure data storage. The application supports **both private and broadcast messaging**, enabling smooth and scalable communication between multiple users.  

The multi-threaded server efficiently manages **simultaneous client connections**, ensuring low-latency message delivery and optimal performance.  

---

## **📌 Features**
✅ **Multi-threaded Server:** Handles multiple client connections concurrently using Boost.Asio.  
✅ **Private & Broadcast Messaging:** Users can send messages privately to a specific client or broadcast messages to all connected users.  
✅ **Secure User Authentication:** Credentials are securely stored in MongoDB with authentication mechanisms.  
✅ **Chat History Storage:** MongoDB ensures persistent storage of messages and logs for easy retrieval.  
✅ **Thread-Safe Database Access:** Implemented Mutex-based control to prevent race conditions.  
✅ **Scalability & Performance:** Optimized for efficient message delivery and system responsiveness.  

---

## **📌 Tech Stack**
- **Programming Language:** C++  
- **Networking:** Winsock & Boost.Asio  
- **Database:** MongoDB  
- **Concurrency Handling:** Multi-threading with Mutex locks  
- **Build System:** CMake (optional)  

---

## **📌 Installation & Setup**
### **1️⃣ Prerequisites**
Ensure you have the following installed on your system:
- **C++ Compiler** (GCC/Clang/MSVC)
- **Boost.Asio** (Networking library)
- **Winsock API** (Windows Sockets)
- **MongoDB** (Database for storing user credentials and chat history)

### **2️⃣ Clone the Repository**
```sh
git clone https://github.com/your-username/chat-application.git
cd chat-application

3️⃣ Install Dependencies
# For Boost
vcpkg install boost-asio

# For MongoDB C++ Driver
vcpkg install mongo-cxx-driver

4️⃣ Compile & Run
g++ -o server server.cpp -lws2_32 -lpthread -lboost_system -lmongocxx
g++ -o client client.cpp -lws2_32 -lboost_system
./server  # Start the server
./client  # Run the client
