#include "FileParser.h"
#include "Benchmark.h"

int main(int argc, char** argv) {
    Benchmark<100>("Read file", &parseFile, "The Mouse Returns.cy").outputTimes();
    auto objects = parseFile("The Mouse Returns.cy");
}
