#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <ctime>
#include <chrono>
#include <fstream>
#include <vector>
#include <csignal>

using namespace std;

struct FileAccessEvent {
    std::string filename;
    std::string timestamp;
    std::string eventType;
    off_t offset;  // Added member for offset
    off_t length;  // Added member for length
};

std::vector<FileAccessEvent> fileAccessEvents;
bool timeToExit = false;

// Function to get file information
void getFileInformation(const std::string& filePath, off_t& offset, off_t& length) {
    int fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    // Calculate the offset to the end of the file
    offset = 0;

    // Calculate the length of the file
    length = lseek(fd, 0, SEEK_END);

    // Close the file
    close(fd);
}

void handleReadEvent(const std::string& path, const std::string& filename) {
    FileAccessEvent event{filename, "", "read", 0, 0};
    auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
    event.timestamp = ctime(&timenow);

    // Calculate file information
    getFileInformation(path + "/" + filename, event.offset, event.length);

    // Print and store file information
    std::cout << "Read event: " << path << "/" << filename  << std::endl;
    std::cout << "Offset: " << event.offset << ", Length: " << event.length << "  time hua hai,  "<< event.timestamp <<endl;

    // Store file access event
    fileAccessEvents.push_back(event);
}

void handleWriteEvent(const std::string& path, const std::string& filename) {
    FileAccessEvent event{filename, "", "write", 0, 0};
    auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
    event.timestamp = ctime(&timenow);

    // Calculate file information
    getFileInformation(path + "/" + filename, event.offset, event.length);

    // Print and store file information
    std::cout << "Write event: " << path << "/" << filename << std::endl;
    std::cout << "Offset: " << event.offset << ", Length: " << event.length << std::endl;

    // Store file access event
    fileAccessEvents.push_back(event);
}

void exportToCSV(const std::string& filePath, const std::string path) {
    std::ofstream outputFile(filePath, std::ios::app);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open CSV file for writing." << std::endl;
        return;
    }

    for (const auto& event : fileAccessEvents) {
        outputFile << event.eventType << ", " <<path <<"/"<< event.filename << ", " << event.offset << ", " << event.length <<", " <<event.timestamp<<"\n";
    }

    outputFile.close();

    if (!outputFile) {
        std::cerr << "Error: Unable to write data to CSV file." << std::endl;
    } else {
        std::cout << "Data successfully written to CSV file." << std::endl;
    }
}

void alarmHandler(int signal) {
    timeToExit = true;
}

int main() {
    std::string fileSystemToMonitor = "/home/abhi/Documents/Veritas/someRepo";

    int inotifyFd = inotify_init();
    if (inotifyFd == -1) {
        perror("Error initializing inotify");
        return 1;
    }

    int watchDescriptor = inotify_add_watch(inotifyFd, fileSystemToMonitor.c_str(), IN_CLOSE_WRITE | IN_CLOSE_NOWRITE);
    if (watchDescriptor == -1) {
        perror("Error adding inotify watch");
        close(inotifyFd);
        return 1;
    }
instance
    char buffer[4096];
    ssize_t bytesRead;

    signal(SIGALRM, alarmHandler);
    alarm(20);

    while (true) {
        bytesRead = read(inotifyFd, buffer, sizeof(buffer));
        if (bytesRead == -1) {
            perror("Error reading inotify events");
            break;
        }

        for (char* ptr = buffer; ptr < buffer + bytesRead;) {
            struct inotify_event* event = reinterpret_cast<struct inotify_event*>(ptr);

            if (event->len > 0) {
                if (event->mask & IN_CLOSE_WRITE) {
                    handleWriteEvent(fileSystemToMonitor, event->name);
                } else if (event->mask & IN_CLOSE_NOWRITE) {
                    handleReadEvent(fileSystemToMonitor, event->name);
                }
            }

            ptr += sizeof(struct inotify_event) + event->len;
        }

        if (timeToExit) {
            break;
        }
    }

    // Export data to CSV before termination
    exportToCSV("/home/abhi/Documents/Veritas/output.csv", fileSystemToMonitor);

    // Clean upinstance
    close(inotifyFd);

    return 0;
}
