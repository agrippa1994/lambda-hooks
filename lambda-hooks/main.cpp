#include <iostream>
#include "detour.h"

int __cdecl add(int x, int y) { return x + y; }
int __stdcall sub(int x, int y) { return x - y; }


struct S
{
	virtual float div(float x, float y)
	{
		return x + y;
	}
} g_s;

Hook<convention_type::stdcall_t, int, int, int> g_subHook;

int main()
{
	Hook<convention_type::cdecl_t, int, int, int>(add, [](int x, int y) -> int
	{
		return (x + y) * 2;
	});

	g_subHook.apply(sub, [](int x, int y) -> int
	{
		return g_subHook.callOrig(x, y) * 3;
	});



	DWORD func = ** (DWORD **) &g_s; // address of the virtual function
	Hook<convention_type::thiscall_t, float, float, float>(func, [](void *obj, float x, float y) -> float{
		return 5 * x;
	});


	std::cout << add(2, 2) << std::endl;
	std::cout << sub(10, 5) << std::endl;
	std::cout << g_s.div(5, 5) << std::endl;

	std::cin.get();
}