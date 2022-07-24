class A
{
};

class FakeA
{
public:
    FakeA(A* a): _a(a)
    {}

    // operator A()
    operator A() const 
    {
        return *_a;
    }

private:
    A* _a;
};

void DoNothingFunc(const A& a)
{
    // .....
}



int main()
{
    A a;
    FakeA fakeA(&a);

    DoNothingFunc(fakeA);

    return 0;
}