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
