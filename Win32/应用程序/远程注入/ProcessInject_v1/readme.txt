该工程有问题：
ProcessFunc.cpp文件中的listAllProcess函数
在for循环中注释那里：

//列表框被创建的时候已经有一行空行。必须先向这一行插入数据，再增加新行

而若是第一次插入，lvitem.iItem的值需要被设为0