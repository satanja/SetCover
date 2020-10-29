#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <vector>

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

        std::set<int> sorted_set;
        while (!ss.eof())
        {
            int x;
            ss >> x;
            sorted_set.insert(x - 1);
        }

        std::vector<int> sorted(sorted_set.begin(), sorted_set.end());
        for (int i = 0; i < sorted.size(); i++)
        {
            if (i != sorted.size() - 1)
            {
                std::cout << sorted[i] << " ";
            }
            else 
            {
                std::cout << sorted[i] << "\n";
            }
        }
    }
}