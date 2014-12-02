#pragma once

#include <unordered_map>
#include <list>
#include <typeindex>
#include <type_traits>

class IEventBusEvent;

/**
 * @brief The handler will get notified for every published IEventBusEvent that it is registered for.
 */
template<class T>
class IEventBusHandler {
private:
	friend class EventBus;
	void dispatch(const IEventBusEvent& e) {
		onEvent(reinterpret_cast<const T &>(e));
	}
public:
	IEventBusHandler() {
		static_assert(std::is_base_of<IEventBusEvent, T>::value, "Wrong type given");
	}

	virtual ~IEventBusHandler() {
	}

	virtual void onEvent(const T&) = 0;
};

/**
 * @brief With a topic you can filter you handler. See the EventBus::subscribe for more details.
 */
class IEventBusTopic {
protected:
	const std::type_index _index;

	IEventBusTopic() :
			_index(typeid(*this)) {
	}
public:
	virtual ~IEventBusTopic() {
	}

	inline bool operator==(const IEventBusTopic& other) const {
		return _index == other._index;
	}
};

class IEventBusEvent {
protected:
	const IEventBusTopic *_topic;

public:
	IEventBusEvent(const IEventBusTopic* const topic = nullptr) :
			_topic(topic) {
	}

	virtual ~IEventBusEvent() {
	}

	inline const IEventBusTopic* getTopic() const {
		return _topic;
	}
};

#define EVENTBUSEVENT(name) class name: public IEventBusEvent { public: name() : IEventBusEvent(nullptr) { } }

class EventBus {
private:
	class EventBusHandlerReference;
	typedef std::list<EventBusHandlerReference> EventBusHandlerReferences;
	typedef std::unordered_map<std::type_index, EventBusHandlerReferences> EventBusHandlerReferenceMap;

	class EventBusHandlerReference {
	private:
		void* const _handler;
		const IEventBusTopic *_topic;

	public:
		EventBusHandlerReference(void* handler, const IEventBusTopic* topic) :
				_handler(handler), _topic(topic) {
		}

		inline IEventBusHandler<IEventBusEvent>* getHandler() const {
			return static_cast<IEventBusHandler<IEventBusEvent>*>(_handler);
		}

		inline const IEventBusTopic* getTopic() const {
			return _topic;
		}
	};

	EventBusHandlerReferenceMap _handlers;

public:
	template<class T>
	void subscribe(IEventBusHandler<T>& handler, const IEventBusTopic* topic = nullptr) {
		const std::type_index& index = typeid(T);
		EventBusHandlerReferences& handlers = _handlers[index];
		const EventBusHandlerReference registration(&handler, topic);
		handlers.push_back(registration);
	}

	template<class T>
	int unsubscribe(IEventBusHandler<T>& handler, const IEventBusTopic* topic = nullptr) {
		int unsubscribedHandlers = 0;
		const std::type_index& index = typeid(T);
		EventBusHandlerReferences& handlers = _handlers[index];
		for (EventBusHandlerReferences::iterator i = handlers.begin(); i != handlers.end();) {
			EventBusHandlerReference& r = *i;
			if (r.getHandler() != reinterpret_cast<IEventBusHandler<IEventBusEvent>*>(&handler)) {
				++i;
				continue;
			}
			if (topic != nullptr && r.getTopic() != nullptr) {
				if (!(*r.getTopic() == *topic)) {
					++i;
					continue;
				}
			}
			i = handlers.erase(i);
			++unsubscribedHandlers;
		}
		return unsubscribedHandlers;
	}

	int publish(const IEventBusEvent& e) {
		const std::type_index& index = typeid(e);
		EventBusHandlerReferenceMap::iterator i = _handlers.find(index);
		if (i == _handlers.end())
			return 0;

		int notifiedHandlers = 0;
		EventBusHandlerReferences& handlers = i->second;
		for (auto& r : handlers) {
			if (r.getTopic() != nullptr) {
				const IEventBusTopic* topic = e.getTopic();
				if (topic == nullptr)
					continue;
				if (!(*r.getTopic() == *topic))
					continue;
			}
			IEventBusHandler<IEventBusEvent>* handler = r.getHandler();
			handler->dispatch(e);
			++notifiedHandlers;
		}
		return notifiedHandlers;
	}
};
