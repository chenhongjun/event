### 观察者模式C++实现

这是设计模式中观察者模式的一种实现。

这是一个简单的C++工具库。

**功能说明**

当你调用Event对象的post函数或者async_post函数时，所有被regist过的Observer对象的update函数都会被调用

你可以继承class Observer来实现你的观察者，并将它们注册到要观察的Event中

你可以用它来解耦你程序中的对象间依赖：被调方(观察者)只需要负责注册，调用方只需要负责调用一次post函数

**example**

```c++
#include "event/event.h"

```

**实现细节说明**

1. 支持传递不同类型的消息
2. 同时支持 同步和异步风格 推送消息
3. 异步推送消息时，通过线程池完成

**注意事项**

- 传递给观察者的message采用std::shared_ptr包装，1是防止，2是防止拷贝，3是提醒大家在程序中所有资源都用std::shared_ptr来管理。当然这样做也有不好的地方，如果你只是想传递一个简单的message，却要想办法把它封装成std::shared_ptr，反而变复杂了。对此，你可以自行对库的实现进行简单修改来实现。
- 没有添加工程文件或者Makefile，因为项目文件太少，直接包含过去就用吧。
- 存在一个bug，多个Event实例时，会共用同一个observer list，待解决。

**解决上述bug**

熬了个通宵将它解决掉了。问题很小，改起来大动干戈。打算把带bug的版本拉一个分支，用来和修复后的版本进行对比。