/* test of number functions' override by const and non-const */
#include <iostream>

class TextBlock
{
public:
    TextBlock(const std::string& str): text(str)
    {}
    const char& operator[] (std::size_t position) const
    { 
        std::cout << "const version!" << std::endl;
        return text[position]; 
    } 
    char& operator[] (std::size_t position) 
    { 
        std::cout << "non-const version" << std::endl;
        return text[position];
    }
private:
    std::string text;
};


int main()
{
    TextBlock tb("Hello");
    std::cout << tb[0] << std::endl;

    const TextBlock ctb("World");
    std::cout << ctb[0] << std::endl;
}