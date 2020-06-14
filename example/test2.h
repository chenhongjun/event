#ifndef EXAMPLE_TEST2_
#define EXAMPLE_TEST2_

#include <chrono>
#include "../event/event.h"

class Observer2 : public Event::Observer<std::string>
{
public:
    using Father = Observer<std::string>;
    virtual void async_update(const std::shared_ptr<std::string>& message);
};

class Observer3 : public Event::Observer<std::uint32_t>
{
public:
    using Father = Event::Observer<std::string>;
    virtual void async_update(const std::shared_ptr<std::uint32_t>& message)
    {
        printf("test3, update %d\n", *message);
    }
};
class Observer4 : public Event::Observer<std::vector<int>>
{
public:
    using Father = Event::Observer<std::vector<int>>;
    virtual void async_update(const std::shared_ptr<std::vector<int>>& message)
    {
        for (auto&& item : *message)
            printf("test4, update %d\n", item);
    }
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

#endif // EXAMPLE_TEST2_