#include <iostream>
#include <string>
#include <vector>

// define interface
class IShape
{
public:
	virtual ~IShape() = default;
	virtual void Draw() const = 0;
};

// implement a box
class Box : public IShape
{
public:
	virtual ~Box() = default;

	virtual void Draw() const override
	{
		std::cout << "Draw Box\n";
	}
};

// implement a triangle
class Triangle : public IShape
{
public:
	virtual ~Triangle() = default;

	virtual void Draw() const override
	{
		std::cout << "Draw Triangle\n";
	}
};

int main()
{
	// build list
	std::vector<IShape*> vObjects;
	vObjects.push_back(new Box());
	vObjects.push_back(new Triangle());

	// draw all obects
	for (const auto pObj : vObjects)
		pObj->Draw();

	// release
	for (auto pObj : vObjects)
		delete pObj;
}
