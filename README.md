### 观察者模式C++实现

这是设计模式中观察者模式的一种实现。

这是一个简单的C++工具库。

**功能说明**

当你调用Event对象的post函数或者async_post函数时，所有被regist过的Observer对象的update函数都会被调用

你可以继承class Observer来实现你的观察者，并将它们注册到要观察的Event中

你可以用它来解耦你程序中的对象间依赖：被调方(观察者)只需要负责注册，调用方只需要负责调用一次post函数

**example**

1位老板招聘了4名程序员，并通过观察者模式进行通信，最后解雇员工的例子：

```c++
#include <chrono>
#include "../event/event.h"

using StringObserver = Event::Observer<std::string>;
class Coder : public StringObserver
{
public:
	Coder(const std::string& name) :name_(name) {}
	~Coder() = default;
	virtual void update(const std::string& message)
	{
		printf("%s: received orders from leader(%s), begin execute...\n", name_.c_str(), message.c_str());
		//to do immediately
	}
	virtual void async_update(const std::shared_ptr<std::string>& message)
	{
		printf("%s: received async orders from leader(%s), begin execute...\n", name_.c_str(), message->c_str());
		//stroke at work
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(2s);
		//to do the command
	}
	std::string name_;
};


class Leader
{
public:
	Leader() { event_ = std::make_shared<Event::Event>(1); }
	void add_staff(const std::shared_ptr<StringObserver>& observer)
	{
		event_->regist(observer);
	}
	void work()
	{
		event_->post(std::string("build a web site."));
		event_->async_post(std::make_shared<std::string>("build again web site."));
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(10s);
	}
	void dismiss_staff(const std::shared_ptr<StringObserver>& observer)
	{
		event_->unregist(observer);
	}
private:
	std::shared_ptr<Event::Event> event_;
};

int main()
{
	std::shared_ptr<Leader> boss = std::make_shared<Leader>();

	std::shared_ptr<Coder> lisa = std::make_shared<Coder>("lisa");
	std::shared_ptr<Coder> alan = std::make_shared<Coder>("alan");
	std::shared_ptr<Coder> bob = std::make_shared<Coder>("bob");
	std::shared_ptr<Coder> leo = std::make_shared<Coder>("leo");
	boss->add_staff(lisa);
	boss->add_staff(alan);
	boss->add_staff(bob);
	boss->add_staff(leo);
	boss->work();
	boss->dismiss_staff(leo);
	boss->dismiss_staff(bob);
	boss->work();
	boss->dismiss_staff(alan);
	boss->dismiss_staff(lisa);
	boss->work();
	return 0;
}
```

**实现细节说明**

1. 支持传递不同类型的消息
2. 同时支持 同步和异步风格 推送消息
3. 异步推送消息时，通过线程池完成

**注意事项**

- 传递给观察者的message采用std::shared_ptr包装，1是防止，2是防止拷贝，3是提醒大家在程序中所有资源都用std::shared_ptr来管理。当然这样做也有不好的地方，如果你只是想传递一个简单的message，却要想办法把它封装成std::shared_ptr，反而变复杂了。对此，你可以自行对库的实现进行简单修改来实现。
- 没有添加工程文件或者Makefile，因为项目文件太少，直接包含过去就用吧。windows和linux均测试通过。
- 如果在一个Event对象析构前没有主动将所有观察者unregist，则有一些内存将永远无法回收，类似于内存泄漏或僵尸进程。
- 存在一个bug，多个Event实例时，会共用同一个observer list。已解决。

**解决上述bug**

熬了个通宵将它解决掉了。问题很小，改起来大动干戈。打算把带bug的版本拉一个分支，用来和修复后的版本进行对比。