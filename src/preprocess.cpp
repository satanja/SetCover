#include <iostream>
#include <sstream>
#include <string>

int main()
{
    int u;
    std::cin >> u;

    std::cout << u << std::endl;

    int fs;
    std::cin >> fs;
    std::cout << fs << std::endl;

    std::string str;
    getline(std::cin, str);

    for (int i = 0; i < fs; i++)
    {

        getline(std::cin, str);
        std::istringstream ss(str);
        while (!ss.eof())
        {
            int x;
            ss >> x;
            std::cout << x - 1 << " ";
        }
		std::cout << "\n";
    }
}