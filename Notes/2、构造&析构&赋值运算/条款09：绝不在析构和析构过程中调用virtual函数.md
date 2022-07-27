# 条款09：绝不在构造和析构过程中调用virtual函数

​		Never call virtual functions during construction or destruction	

​	考虑这样一个例子，其需要在构造函数中调用 virtual 函数：

````c++
class Transaction{
public:
	Transaction();
    virtual void logTransaction() const = 0; // 做出一份因类型不同的日志记录
};
Transaction::Transaction()
{
    ...
    logTransaction();			// 最后的动作是志记这笔交易
}
class BuyTransaction: public Transaction{
public:
    virtual void logTransaction() const;		// log 这种交易
};
class SelfTransaction: public Transaction{
public:
	virtual void logTransaction() const;		// log 这z
};
````

​	这样的问题是，我们创建一个子类，总是先会调用其基类的构造函数，那么此时的 `logTransaction` 就不会下降到 `derived classes` 阶层，因为此时所有子类特有的对象都还没初始化，所以C++不允许你去调用子类的 `logTransaction`重写版本；

---



​	如果真的有这种需求，就需要在子类的构造函数中传入参数给父类：

````c++
class Transaction{
public:
	explicit Transaction(const std::string& logInfo);
	void logTransaction(const std::string& logInfo) const;
	...
};
Transaction::Transaction(const std::string& logInfo)
{
	...
	logTransaction(logInfo);			// 在构造函数中调用一个 non-virtual 函数
}
class BuyTransaction: public Transaction{
public:
	BuyTransaction(parameters) : Transaction(createLogString( parameters ))
	{...}
	...
private:
	static std::string createLogString( parameters );
};
````

---



## 请记住

- 在构造函数和析构期间不要调用 virtual 函数，因为这类调用从不下降到 derived class （比起当前执行构造函数与析构函数的那层）；



