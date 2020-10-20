bool operator<(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
    return lhs.second < rhs.second;
}

bool operator>(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {
    return lhs.second > rhs.second;
}