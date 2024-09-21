// RandomFileIO.cpp

// RandomFileIO.cpp is a CPP program for performing a specified number of random read / write operations on the files present
// within a specified file system. Only one operation (read/write) will be performed on a file at one time
// stance on any file present. Script.cpp program will then tap these read/write operations to log the 
// training data into the output.csv file. 

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include <filesystem>

namespace fs = std::filesystem;

// Function to perform random read/write operations on files
void randomFileOperation(const std::string& directory, std::mutex& fileMutex) {
    // Get a list of all files in the specified directory
    std::vector<std::string> fileList;
    for (const auto& entry : fs::directory_iterator(directory)) {
        fileList.push_back(entry.path().string());
    }

    if (fileList.empty()) {
        std::cerr << "No files found in the directory: " << directory << std::endl;
        return;
    }

    // Generate a random index to choose a file
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, fileList.size() - 1);
    int fileIndex = dis(gen);

    // Create file name
    std::string fileName = fileList[fileIndex];

    // Perform file operation
    fileMutex.lock(); // Lock the file for exclusive access
    std::fstream file;

    // Randomly choose to read or write
    std::uniform_int_distribution<> rwDis(0, 1);
    int operationType = rwDis(gen);

    if (operationType == 0) {
        // Read operation
        file.open(fileName, std::ios::in);
        if (file.is_open()) {
            std::string content;
            std::getline(file, content);
            std::cout << "Read from " << fileName << ": " << content << std::endl;
            file.close();
        } else {
            std::cerr << "Error opening file for reading: " << fileName << std::endl;
        }
    } else {
        // Write operation
        file.open(fileName, std::ios::out | std::ios::app);
        if (file.is_open()) {
            std::string data = "Random data to write.";
            file << data << std::endl;
            std::cout << "Write to " << fileName << ": " << data << std::endl;
            file.close();
        } else {
            std::cerr << "Error opening file for writing: " << fileName << std::endl;
        }
    }

    fileMutex.unlock(); // Release the lock
}

int main() {
    const std::string directory = "/home/abhi/Documents/Veritas/someRepo";
    const int numOperations = 10; // Number of random read/write operations

    std::mutex fileMutex;

    // Create threads to perform random file operations
    std::vector<std::thread> threads;
    for (int i = 0; i < numOperations; ++i) {
        threads.emplace_back(randomFileOperation, std::ref(directory), std::ref(fileMutex));
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
