#pragma once

#include <functional>

namespace Capp {

	enum class EventType : unsigned int {
		None = 0,

		WindowClosed,
		WindowResized,

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
	
	#if CAPP_DEBUG || CAPP_RELEASE
	
	#define EVENT_CLASS_TYPE(type)\
		static EventType getStaticType() { return EventType::##type; }\
		virtual EventType getEventType() const override { return getStaticType(); }\
		virtual const char* getName() const override { return #type; }

	#else

	#define EVENT_CLASS_TYPE(type)\
		static EventType getStaticType() { return EventType::##type; }\
		virtual EventType getEventType() const override { return getStaticType(); }

	#endif
	
	class Event {

		friend class EventDispatcher;

	public:

		virtual EventType getEventType() const = 0;

		bool isHandled() const;

		#if CAPP_DEBUG || CAPP_RELEASE

		virtual const char* getName() const = 0;
		virtual std::string toString() const;
		
		#endif

	protected:

		Event() = default;
		virtual ~Event() = default;

		bool _isHandled = false;
		
	};

	#if CAPP_DEBUG || CAPP_RELEASE
	inline std::ostream& operator<<(std::ostream& out, const Event& e) { return out << e.toString(); }
	#endif
	
	class EventDispatcher {

		template<typename EType>
		using EventFunction = std::function<bool(EType&)>;

	public:

		EventDispatcher(Event& e);

		template<typename EType, typename = typename std::enable_if<std::is_base_of<Event, EType>::value>::type>
		bool dispatchEventType(EventFunction<EType> function);

	private:

		Event& _event;
		
	};

	template <typename EType, typename>
	bool EventDispatcher::dispatchEventType(EventFunction<EType> function) {
		if(_event.getEventType() == EType::getStaticType()) {
			EType e = *static_cast<EType*>(&_event);
			_event._isHandled = function(e);
			return true;
		}

		return false;
	}
}