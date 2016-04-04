#ifndef TIMER_MANAGER_HPP_
#define TIMER_MANAGER_HPP_

#include <functional>
#include <memory>
#include <list>
#include <algorithm>
#include <chrono>

class timer_manager
{
public:

	class timer;

	using timer_ptr_t = std::shared_ptr<timer>;
	using func_t = const std::function<void(const timer_ptr_t&)>;

	timer_manager(void) = delete;

	class timer : public std::enable_shared_from_this<timer>
	{
	public:

		timer(void) = delete;
		timer(const long long &delay, func_t &func) :
			_time(std::chrono::steady_clock::now()),
			_delay(delay),
			_callback(func) {};

		void enable(void)
		{
			if (!_enable)
			{
				_time = std::chrono::steady_clock::now();

				_enable = true;
			}
		}

		void disable(void)
		{
			if (_enable)
				_enable = false;
		}

		void process(void)
		{
			if (_enable)
			{
				auto &time_span = std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::steady_clock::now() - _time);

				if (time_span.count() > _delay)
				{
					_time = std::chrono::steady_clock::now();

					_callback(shared_from_this());
				}
			}
		}

	private:

		std::chrono::steady_clock::time_point _time;
		long long _delay;
		func_t _callback;
		bool _enable = true;;
	};

	static auto &get_timers(void)
	{
		static std::list<timer_ptr_t> timers;

		return timers;
	}

	static void clear(void)
	{
		get_timers().clear();
	}

	static auto add_timer(const long long &delay, func_t &func)
	{
		auto &new_timer = std::make_shared<timer>(delay, func);

		get_timers().push_back(new_timer);

		return new_timer;
	}

	static void remove_timer(const timer_ptr_t &timer_ptr)
	{
		auto &timers = get_timers();

		timers.erase(std::remove_if(timers.begin(), timers.end(),
			[&timer_ptr](const timer_ptr_t &timer_ptr_comp)
		{
			return timer_ptr.get() == timer_ptr_comp.get();
		}), timers.end());
	}

	static bool process(void)
	{
		auto &timers = get_timers();

		for (auto &timer : timers)
			timer->process();

		return !timers.empty();
	}
};

#endif // TIMER_MANAGER_HPP_