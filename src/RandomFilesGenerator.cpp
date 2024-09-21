// RandomFilesGenerator.cpp

// RandomFilesGenerator is a CPP program for generating random files of different file types
// in a specified file system or mount point. 

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

std::string generateRandomFilename(int length) {
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string randomFilename;
    for (int i = 0; i < length; ++i) {
        randomFilename += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return randomFilename;
}

int main() {
    // Specify the directory where you want to create the random files
    std::string outputDirectory = "/home/abhi/Documents/Veritas/someRepo";

    // Create the directory if it doesn't exist
    //system(("mkdir -p " + outputDirectory).c_str());

    // Number of random files to create
    int numFiles = 100;

    // Loop to create random f
    // Loop to create random files
    for (int i = 0; i < numFiles; ++i) {
        // Generate a random filename
        std::string filename = generateRandomFilename(10);

        // Randomly choose the file extension (.txt or .cpp)
        const char* extensions[] = {".txt", ".dat", ".bin", ".xml", ".json", ".csv"};
        std::string extension = extensions[rand() % (sizeof(extensions) / sizeof(extensions[0]))];

        // Create the random file with some content
        std::ofstream file(outputDirectory + "/" + filename + extension);
        file << "This is a random " << extension.substr(1) << " file." << std::endl;
        file.close();

        std::cout << "Created: " << outputDirectory << "/" << filename << extension << std::endl;
    }

    

    return 0;
}
