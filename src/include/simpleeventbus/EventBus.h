#pragma once

#include <unordered_map>
#include <list>
#include <typeindex>
#include <type_traits>

class IEventBusEvent;

template<class T>
class IEventBusHandler {
public:
	IEventBusHandler() {
		static_assert(std::is_base_of<IEventBusEvent, T>::value, "Wrong type given");
	}

	virtual ~IEventBusHandler() {
	}

	virtual void onEvent(const T&) = 0;

	void dispatch(const IEventBusEvent& e) {
		onEvent(reinterpret_cast<const T &>(e));
	}
};

class IEventBusTopic {
public:
	virtual ~IEventBusTopic() {
	}
};

class IEventBusEvent {
private:
	const IEventBusTopic* _topic;

public:
	IEventBusEvent(const IEventBusTopic* const topic) :
			_topic(topic) {
	}

	virtual ~IEventBusEvent() {
	}

	inline const void* getTopic() const {
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
		const IEventBusTopic* const _topic;

	public:
		EventBusHandlerReference(void* handler, const IEventBusTopic* const topic) :
				_handler(handler), _topic(topic) {
		}

		inline IEventBusHandler<IEventBusEvent>* getHandler() const {
			return static_cast<IEventBusHandler<IEventBusEvent>*>(_handler);
		}

		inline const void* getTopic() const {
			return _topic;
		}
	};

	EventBusHandlerReferenceMap _handlers;

public:
	template<class T>
	void subscribe(IEventBusHandler<T>& handler, IEventBusTopic* topic) {
		const std::type_index& index = typeid(T);
		EventBusHandlerReferences& handlers = _handlers[index];
		const EventBusHandlerReference registration(&handler, topic);
		handlers.push_back(registration);
	}

	template<class T>
	void subscribe(IEventBusHandler<T>& handler) {
		subscribe(handler, nullptr);
	}

	template<class T>
	void unsubscribe(IEventBusHandler<T>& handler, IEventBusTopic* topic) {
		const std::type_index& index = typeid(T);
		EventBusHandlerReferences& handlers = _handlers[index];
		for (EventBusHandlerReferences::iterator i = handlers.begin(); i != handlers.end(); ++i) {
			EventBusHandlerReference& r = *i;
			if (r.getHandler() != reinterpret_cast<IEventBusHandler<IEventBusEvent>*>(&handler))
				continue;
			if (topic != nullptr && r.getTopic() != topic)
				continue;
			i = handlers.erase(i);
		}
	}

	template<class T>
	void unsubscribe(IEventBusHandler<T>& handler) {
		unsubscribe(handler, nullptr);
	}

	int publish(const IEventBusEvent& e) {
		const std::type_index& index = typeid(e);
		EventBusHandlerReferenceMap::iterator i = _handlers.find(index);
		if (i == _handlers.end())
			return 0;

		int notifiedHandlers = 0;
		EventBusHandlerReferences& handlers = i->second;
		for (auto& r : handlers) {
			if (r.getTopic() != nullptr && r.getTopic() != e.getTopic())
				continue;
			IEventBusHandler<IEventBusEvent>* handler = r.getHandler();
			handler->dispatch(e);
			++notifiedHandlers;
		}
		return notifiedHandlers;
	}
};
