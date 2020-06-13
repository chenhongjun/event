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

