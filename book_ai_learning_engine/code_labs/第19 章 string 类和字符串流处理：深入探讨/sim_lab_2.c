#include <string>
int main() {
    volatile std::string s = "short";       // SSO，不应调 malloc
    volatile std::string t = "a long string that will exceed SSO buffer for sure";
    std::string u = std::move(t);           // 移动，不应调 malloc
    return 0;
}