#include <gtest/gtest.h>
#include <EventBus.h>

EVENTBUSEVENT(TestEvent);

class HandlerTest: public IEventBusHandler<TestEvent> {
private:
	int _count;
public:
	HandlerTest() : _count(0) {}

	void onEvent(const TestEvent&) override {
		++_count;
	}

	inline int getCount() const {
		return _count;
	}
};

TEST(EventBusTest, testSubscribeAndPublish) {
	EventBus eventBus;
	HandlerTest handler;
	TestEvent event;

	eventBus.subscribe(handler);
	ASSERT_EQ(1, eventBus.publish(event)) << "Expected one handler to be notified";
	ASSERT_EQ(1, handler.getCount()) << "Expected the handler to be notified once";

	eventBus.unsubscribe(handler);
	ASSERT_EQ(0, eventBus.publish(event)) << "Expected no handler to be notified";
	ASSERT_EQ(1, handler.getCount()) << "Expected the handler not to be notified twice because we unsubscribed it before we published the event";
}
