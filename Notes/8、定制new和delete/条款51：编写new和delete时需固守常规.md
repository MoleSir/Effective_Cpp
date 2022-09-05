# 条款 51：编写 new 和 delete 时需固守常规

​		Adhere to convention when writing new and delete.

编写自己的 `operator new` 与 `operator delete`时需要遵守一定规则；

对 `operator new` 来说：

- 申请成功必须返回正确的值；
- 内部不足时必须调用 new-handling 函数；
- 必须对付 0 内存需求的准备，C++ 规定，即使客户要求 0 字节，也需要按照 1 个字节的空间来申请；
- 还需要避免不慎掩盖正常形式的 new；
- 如果没有能力获取到内存，抛出 `std::bad_alloc` 异常；

可以看这段伪码：

````c++
void* operator new (std::size_t size) throw(std::bad_alloc)
{
    using namespace std;
    if (size == 0)
    {
        size = 1;			// 处理 0 字节要求
	}
    while (true)
    {
        尝试配合 size 个字节
        if (分配成功)
           	return 指向分配所得内存的指针;
        // 分配失败，找出目前的 new-handler 执行
        new_handler globalHandler = set_new_handler(NULL);
        set_new_handler(globalHandler);
        
        if (globalHandler) (*gloablHandler)();
        	// 如果连处理函数都没有，说明，真的没有办法了，抛出异常
        	else throw std::bad_alloc();
	}
}
````

还需要注意的是，`operator new` 会被继承，当父类存在 `operator new ` 的重写，但子类没有时，调用子类的 `new` 可能最后调用了父类的 `operator new`，使得要申请的内存与实际不一致（子类大小通常大于父类）；

可以在进入函数之前判断：

````c++
void* Base::operator* (std::size_t size) throw(std::bad_alloc)
{
	if (size != sizeof(Base))
        	return ::operator new (size);
	...
}
````

并且这个版本同样也对 0 需求做了处理，因为 `sizeof(Base)` 不可能是 0；



而对 `operator delete` 就更简单，要保证内存可以被正确释放：

````c++
void operator delete (void* rawMemory) throw()
{
    if (rawMemory == 0) return;
    归还内存
}
````

`operator delete` 也面临继承的问题，所以需要判断空间大小：

````c++
void Base::operator delete (void* rawMemory, std::size_t size) throw()
{
	if (rawMemory == 0) return;
	if (size != sizeof(Base))
	{
    	::operator delete(rawMemory);
    	return;
	}
	...
}
````



## 请记住

- `operator new` 应该包含一个无穷循环，并在其中尝试分配内存，如果无法满足内存需求，就该调用 `new-handler`。它还应该有能力处理 0 字节申请。Class 专属版本则还应该处理 “比正确大小更大的错误申请”；
- `operator delete` 应该在收到 null 指针是不做任何事情。Class 专属版本应该处理 “比正确大小更大的（错误）申请”；