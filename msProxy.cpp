#include <iostream>
#include <string>
#include <vector>

#include "proxy.h"

// implement a box
class Box
{
public:
	virtual ~Box() = default;

	virtual void Draw() const
	{
		std::cout << "Draw Box\n";
	}
};

// implement a triangle
class Triangle
{
public:
	virtual ~Triangle() = default;

	virtual void Draw() const
	{
		std::cout << "Draw Triangle\n";
	}
};

// define interface with proxy
PRO_DEF_MEM_DISPATCH(ProxyDraw, Draw);

struct ShapeProxy : pro::facade_builder
	::add_convention<ProxyDraw, void()const>
	::build {};

int main()
{
	// build list
	std::vector<pro::proxy<ShapeProxy>> vObjects;
	vObjects.push_back(pro::make_proxy<ShapeProxy,Box>());
	vObjects.push_back(pro::make_proxy<ShapeProxy,Triangle>());

	// draw all objects
	for (const auto& pObj : vObjects)
		pObj->Draw();
}
