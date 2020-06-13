#pragma once
#include "../event/event.h"
class Observer2 : public Event::Observer<std::string>
{
public:
    using Father = Observer<std::string>;
    virtual void update(const std::shared_ptr<std::string>& message);
};


class Test2
{
public:
    Test2() { event_bus_ = std::make_shared<Event::Event>(); }
    void regist(const std::shared_ptr<Observer2>& observer);
    void test();
    void unregist(const std::shared_ptr<Observer2>& observer);
private:
    std::shared_ptr<Event::Event> event_bus_;
};
