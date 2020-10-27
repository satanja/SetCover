# Set Cover Reducer
This project reduces Set Cover instances using two simple reductions, namely
1. include families with a unique element,
1. discard sets that are a subset of another set.

Given the reduction subroutine, we can also improve the simple greedy algorithm. We only make greedy choices when absolutely necessary, i.e., we can no longer reduce the instance. After making a greedy choice, we reduce the new instance again. This approach to the greedy algorithm maintains the O(log n) approximation bound, but often yields better solution sizes.

The performance of the tool is rather decent, we can expect to reduce over 300 thousand sets in a couple of minutes.

## Requirements
This project uses CMake and the C++20 STL. CMake can be downloaded [here](https://cmake.org/download/]).
Alternatively, CMake can be installed with Visual Studio. Microsoft has given the [following guide])(https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=vs-2019) for Visual Studio 2019.

## Building
TODO.

## Usage
Copy the binary to `inputs/`, and in command prompt (or a terminal) use `setcover [mode] [input.dat]`.
Here, `[mode]` is either `0` or `1` for reducing the input once or using the greedy reduce algorithm on the instance respectively, and [input.dat] is the selected input, e.g., `chess.dat`. 