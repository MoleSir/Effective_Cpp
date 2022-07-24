

#define CLAUSE_10 "Have assignment operator return a reference to this"


namespace test
{
// assignment operator is linelock

    class Widget
    {
        //...//
    public:
        Widget& operator += (const Widget& that)
        {
            // ... 
            return *this;
        }

        Widget& operator = (const Widget& that)
        {
            // ...
            return *this;
        }
    };


    void test()
    {
        int a, b, c, d;
        a = b = d = c = 15;
    }
}


int main()
{


    return 0;
}