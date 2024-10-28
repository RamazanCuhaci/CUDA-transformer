#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <filesystem>

void generateCombinations(const std::vector<int> &options, std::vector<int> &combination, int index,
                          std::vector<std::string> &output)
{
    if (index == options.size())
    {
        std::stringstream stringStream;
        for (int val : combination)
        {
            stringStream << val;
        }
        output.push_back(stringStream.str());
        return;
    }

    for (int i = 0; i < options[index]; ++i)
    {
        combination[index] = i;
        generateCombinations(options, combination, index + 1, output);
    }
}

int main(int argc, char *argv[])
{

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    /// Check if a command line argument was provided
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <command>" << std::endl;
        return 1;
    }

    /// The command to run is passed as the first argument
    std::string fileName = argv[1];
    std::string cmd = "CUDA-transformer --analyze " + fileName + " -- --cuda-gpu-arch=sm_86";

    /// Create a pipe to run the command
    std::array<char, 128> buffer;
    std::stringstream result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);

    if (!pipe)
    {
        std::cerr << "Failed to run command.\n";
        return 1;
    }

    /// Read the output of the command
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result << buffer.data();
    }
    std::string output = result.str();

    std::vector<int> optionCounts;
    /// Number of options for each type of optimization
    std::vector<int> typeOptions = {5, 3, 2, 3, 3, 5, 5, 3, 4, 5};

    /// Parse the output to get the optimization possibilities
    for (char c : output)
    {
        if (isdigit(c))
        {
            int typeIndex = c - '0';
            optionCounts.push_back(typeOptions[typeIndex]);
        }
    }

    /// Generate all possible combinations of optimization choices
    std::vector<int> combination(optionCounts.size(), 0);
    std::vector<std::string> outputStrings;
    generateCombinations(optionCounts, combination, 0, outputStrings);


    /// For polybench include options or other include options
    std::string includeOptions;
    if (argv[2])
    {
        includeOptions = argv[2];
    }
    
    /// Clean up
    if (std::filesystem::exists("results"))
    {
        std::filesystem::remove_all("results");
    }
    std::filesystem::create_directory("results");
    
    #pragma omp parallel
    #pragma omp for
    for (const std::string &choices : outputStrings)
    {
        std::cout << "Processing options " + choices +"...\n";
        std::string outputFilename = "-o results/" + choices + ".cu ";
        std::string newCmd =
            "CUDA-transformer --choices=" + choices + " " + outputFilename + fileName + " -- " + includeOptions +" --cuda-gpu-arch=sm_86 &";
        std::system(newCmd.c_str());
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "\nOptimization types: " << output << std::endl;
    std::cout << "Time taken = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;


    return 0;
}
