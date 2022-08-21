#include <iostream>

namespace my_std
{
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};


    class list_iterator
    {
    public:
        using iterator_category = bidirectional_iterator_tag;
    };


    template <typename IterType>
    class iterator_trait
    {
    public:
        using iterator_category = typename IterType::iterator_category;
    };

    template <typename IterType>
    class iterator_trait<IterType*>
    {
    public:
        using iterator_category = random_access_iterator_tag;
    };


    template <typename IterType, typename DistType>
    void DoAdvance(IterType iter, DistType distance, bidirectional_iterator_tag)
    {
        std::cout << "bidirectional_iterator_tag" << std::endl;
    }

    template <typename IterType, typename DistType>
    void DoAdvance(IterType iter, DistType distance, random_access_iterator_tag)
    {
        std::cout << "random_access_iterator_tag" << std::endl;
    }

    template <typename IterType, typename DistType>
    void Advance(IterType iter, DistType distance)
    {
        using iterator_category = typename iterator_trait<IterType>::iterator_category;
        DoAdvance(iter, distance, iterator_category());
    }
}

int main()
{
    using namespace my_std;
    list_iterator iter1;
    int* iter2 = nullptr;
    Advance(iter1, 2);
    Advance(iter2, 232);
}