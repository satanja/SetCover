# Set Cover Reducer
This project reduces Set Cover instances using two simple reductions, namely
1. include sets with a unique element,
1. discard sets that are a subset of another set.

Given the reduction subroutine, we can also improve the simple greedy algorithm. We only make greedy choices when absolutely necessary, i.e., when we can no longer reduce the instance. After making a greedy choice, we reduce the new instance again. This approach to the greedy algorithm maintains the O(log n) approximation bound, but often yields better solution sizes.

The performance of the tool is rather decent, we can expect to reduce over 300 thousand sets in a couple of minutes.

## Requirements
This project uses CMake and the C++20 STL. CMake can be downloaded [here](https://cmake.org/download/]).
Alternatively, CMake can be installed with Visual Studio. Microsoft has given the [following guide](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=vs-2019) for Visual Studio 2019.

## Building
TODO.

## Usage

### Input Specification
The tool requires inputs to have the following specification. 
* The first line contains an integer `n` with the elements in the universe numbered from `0` to `n - 1`.
* The second line contains an integer `m` with the sets numbered from `0` to `m - 1`.
* Line `k + 3`, with `0 <= k < m`, contains the elements of set `k` in ascending order separated by spaces, but with no space after the last element.

*The tool has undefined behavior in case there is a space after the last element of each set*.

### Running the tool
Copy the binary to `inputs/`, and in command prompt (or a terminal) use `setcover [mode] [input]`.
Here, `[mode]` is either `0` or `1` for reducing the input once or using the greedy reduce algorithm on the instance respectively, and `[input]` is the selected input, e.g., `chess.dat`. Note that when the greedy reduce algorithm is invoked, a solution will be written with the name `[input]-solution.txt`.