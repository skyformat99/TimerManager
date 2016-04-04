# TimerManager
Crossplatform library for simple working with timers
# Example
```c++
#include <iostream>
#include "timer_manager.hpp"

using namespace std;

void main(void)
{
	timer_manager::add_timer(5000,
		[](auto &timer_ptr)
	{
		cout << "Hello, World!" << endl;

		timer_manager::remove_timer(timer_ptr);
	});

	while (true)
	{
		if (!timer_manager::process())
			break;
	}

	cin.get();
}
```
