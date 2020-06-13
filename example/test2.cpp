#include "test2.h"
class Observer3 : public Observer<std::string>
{
public:
	using Father = Observer<std::string>;
	virtual void update(const std::shared_ptr<std::string>& message) {}
};
class Observer4 : public Observer<std::uint32_t>
{
public:
	using Father = Observer<std::string>;
	virtual void update(const std::shared_ptr<std::string>& message) {}
};
void Observer2::update(const std::shared_ptr<std::string>& message)
{
	printf("test2, update %s", message->c_str());
}
void Test2::regist(const std::shared_ptr<Observer2>& observer)
{
	event_bus_->regist(std::dynamic_pointer_cast<Observer2::Father>(observer));
}
void Test2::test()
{
	event_bus_->post(std::make_shared<std::string>("123"));
}
void Test2::unregist(const std::shared_ptr<Observer2>& observer)
{
	event_bus_->unregist(std::dynamic_pointer_cast<Observer2::Father>(observer));
}