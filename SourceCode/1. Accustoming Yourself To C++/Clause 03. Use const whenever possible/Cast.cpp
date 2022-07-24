/* use cast to simplidfy code */  
/* use const to realize non-const */
#include <iostream>
#include <string>


class TextBlock
{
public:
    TextBlock(const std::string& str): text(str)
    {}
    const char& operator[] (std::size_t position) const
    { 
        return text[position]; 
    } 
    char& operator[] (std::size_t position) 
    { 
        return const_cast<char&>( static_cast<const TextBlock& >(*this)[position] );
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