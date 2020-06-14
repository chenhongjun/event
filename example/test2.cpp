#include "../example/test2.h"

void Observer2::async_update(const std::shared_ptr<std::string>& message)
{
    printf("test2, update %s\n", message->c_str());
    printf("sleep begin\n");
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    printf("sleep end\n");
}
void Test2::regist(const std::shared_ptr<Observer2>& observer)
{
    event_bus_->regist(std::dynamic_pointer_cast<Observer2::Father>(observer));
}
void Test2::test()
{
    event_bus_->async_post(std::make_shared<std::string>("123"));
}
void Test2::unregist(const std::shared_ptr<Observer2>& observer)
{
    event_bus_->unregist(std::dynamic_pointer_cast<Observer2::Father>(observer));
}

