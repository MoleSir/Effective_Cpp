# 条款 50：了解 new 和 delete 的合理替换时机

​		Understand when it makes sense to replace new and delete.

替换编译器默认的 `operator new`  与 `operator delete` 有这些理由：

- **用来检测运用上的错误**；

- **为了强化效能**：编译器自带的  `operator new`  与 `operator delete`  用于一般目的，需要适用一些了需求，申请大内存、小块内存、大小混合内存；

    对自己的需求定制  `operator new`  与 `operator delete`  可以有更快的执行速度，更高的空间利用率；

- **为了收集使用上的统计数据**：定制  `operator new`  与 `operator delete` ，在其中收集信息；

- **为了增加分配和归还的速度**；

- **为了降低缺省内存管理器带来的空间额外开销**；

- **为了弥补缺省分频器中的非最佳齐位**；

- **为了将相关对象成簇集中**；

- **为了获得非传动的行为**；



## 请记住

- 有许多理由需要些个自定义的 new 和 delete，包括改善效能、对 heap 运用错误进行调试、收集 heap 适用信息；

