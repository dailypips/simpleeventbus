#include <EventBus.h>

#include <cstdlib>
#include <iostream>

EVENTBUSEVENT(TestEvent);

class HandlerTest: public IEventBusHandler<TestEvent> {
private:
	bool _success;
public:
	HandlerTest() : _success(false) {}

	void onEvent(const TestEvent&) override {
		_success ^= true;
	}

	inline bool isSuccess() const {
		return _success;
	}
};

/**
 * @brief Simple example of the EventBus usage. Also make sure to check
 * out the included unittests to get more examples.
 */
int main(const int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	EventBus eventBus;
	HandlerTest handler;
	TestEvent event;

	eventBus.subscribe(handler);
	eventBus.publish(event);

	if (!handler.isSuccess()) {
		std::cerr << "Failed to execute the handler" << std::endl;
		return EXIT_FAILURE;
	}

	eventBus.unsubscribe(handler);
	eventBus.publish(event);
	if (!handler.isSuccess()) {
		std::cerr << "Failed to unsubscribe" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Executed the handler" << std::endl;
	return EXIT_SUCCESS;
}
