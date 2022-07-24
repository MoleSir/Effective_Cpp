# 条款19：设计class犹如设计type

​	Treat class design as type design

​	当你定义一个新的 class，也就是在定义一个新的 type；

​	设计时，需要这样提问自己：

- 新 type 的对象应该如何被创建和销毁？
- 对象的初始化和对象的赋值该有什么差别？
- 新 type 对象如果被 passed by value（以值传递），意味着什么？
- 什么是新 type 的合法值？
- 你的新 type 需要配合某个继承图系吗？
- 你的新 type 需要什么样的转换？
- 什么样的操作符和函数对此新 type 而言是合理的？
- 什么样的标志函数应该被驳回？
- 谁该取用新 type 的成员？
- 什么是新 type 的“未声明接口”？
- 你的 type 有多一般化？