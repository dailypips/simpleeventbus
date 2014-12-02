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

TEST(EventBusTest, testSubscribeAndPublish_1) {
	EventBus eventBus;
	HandlerTest handler;
	TestEvent event;

	eventBus.subscribe(handler);
	ASSERT_EQ(1, eventBus.publish(event)) << "Expected one handler to be notified";
	ASSERT_EQ(1, handler.getCount()) << "Expected the handler to be notified once";

	ASSERT_EQ(1, eventBus.unsubscribe(handler));
	ASSERT_EQ(0, eventBus.publish(event)) << "Expected no handler to be notified";
	ASSERT_EQ(1, handler.getCount()) << "Expected the handler not to be notified twice because we unsubscribed it before we published the event";
}

TEST(EventBusTest, testMassSubscribeAndPublish_10000000) {
	EventBus eventBus;
	HandlerTest handler;
	TestEvent event;

	const int n = 10000000;
	for (int i = 0; i < n; ++i) {
		eventBus.subscribe(handler);
	}
	ASSERT_EQ(n, eventBus.publish(event)) << "Unexpected amount of handlers notified";
	ASSERT_EQ(n, handler.getCount()) << "Unexpected handler notification amount";

	ASSERT_EQ(n, eventBus.unsubscribe(handler));
	ASSERT_EQ(0, eventBus.publish(event)) << "Expected no handler to be notified";
	ASSERT_EQ(n, handler.getCount()) << "Expected the handler not to be notified again because we unsubscribed it before we published the event";
}

TEST(EventBusTest, testSubscribeAndUnsubscribe_1000) {
	EventBus eventBus;
	HandlerTest handler;

	const int n = 1000;
	for (int i = 0; i < n; ++i) {
		eventBus.subscribe(handler);
	}
	ASSERT_EQ(n, eventBus.unsubscribe(handler));
}

TEST(EventBusTest, testMassPublish_10000000) {
	EventBus eventBus;
	HandlerTest handler;
	TestEvent event;

	eventBus.subscribe(handler);
	const int n = 10000000;
	for (int i = 0; i < n; ++i) {
		ASSERT_EQ(1, eventBus.publish(event)) << "Unexpected amount of handlers notified";
		ASSERT_EQ(i + 1, handler.getCount()) << "Unexpected handler notification amount";
	}

	ASSERT_EQ(1, eventBus.unsubscribe(handler));
	ASSERT_EQ(0, eventBus.publish(event)) << "Expected no handler to be notified";
	ASSERT_EQ(n, handler.getCount()) << "Expected the handler not to be notified again because we unsubscribed it before we published the event";
}
