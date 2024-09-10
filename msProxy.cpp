#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "proxy.h"

template<typename TFunc>
inline size_t benchmark(TFunc test, size_t uTimes)
{
	using TClock = std::chrono::high_resolution_clock;
	auto tpStart = TClock::now();

	for (size_t t = 0; t < uTimes; ++t)
		test();

	return std::chrono::duration_cast<std::chrono::milliseconds>(TClock::now() - tpStart).count();
}

namespace nsInheritance
{
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
			++uCounter;
		}

	public:
		static inline size_t uCounter = 0;
	};

	// implement a triangle
	class Triangle : public IShape
	{
	public:
		virtual ~Triangle() = default;

		virtual void Draw() const override
		{
			++uCounter;
		}

	public:
		static inline size_t uCounter = 0;
	};
}

namespace nsNormalClass
{
	// implement a box
	class Box
	{
	public:
		void Draw() const
		{
			++uCounter;
		}

	public:
		static inline size_t uCounter = 0;
	};

	// implement a triangle
	class Triangle
	{
	public:
		void Draw() const
		{
			++uCounter;
		}

	public:
		static inline size_t uCounter = 0;
	};
}

// define interface with proxy
PRO_DEF_MEM_DISPATCH(ProxyDraw, Draw);

struct ShapeProxy : pro::facade_builder
	::add_convention<ProxyDraw, void()const>
	::build {};

int main()
{
	size_t numOfTest = 100000;
	size_t numOfEachObj = 1000;

	// test inheritance
	{
		using namespace nsInheritance;
		std::vector<IShape*> vObj;
		vObj.reserve(numOfEachObj * 2);
		for (size_t i = 0; i < numOfEachObj; ++i)
		{
			vObj.emplace_back(new Box());
			vObj.emplace_back(new Triangle());
		}

		std::cout << "inheritance: " << benchmark(
			[&vObj]() {
				for (const auto& obj : vObj)
					obj->Draw();
			}, numOfTest) << "\n";

		std::cout << Box::uCounter << " / " << Triangle::uCounter << "\n\n";

		for (auto& obj : vObj)
			delete obj;
	}

	// test proxy
	{
		using namespace nsNormalClass;
		std::vector<pro::proxy<ShapeProxy>> vObj;
		vObj.reserve(numOfEachObj * 2);
		for (size_t i = 0; i < numOfEachObj; ++i)
		{
			vObj.emplace_back(pro::make_proxy<ShapeProxy, Box>());
			vObj.emplace_back(pro::make_proxy<ShapeProxy, Triangle>());
		}

		std::cout << "using proxy: " << benchmark(
			[&vObj]() {
				for (const auto& obj : vObj)
					obj->Draw();
			}, numOfTest) << "\n";

		std::cout << Box::uCounter << " / " << Triangle::uCounter << "\n\n";
	}

	// call directly
	{
		using namespace nsInheritance;
		std::vector<Box*> vBox;
		vBox.reserve(numOfEachObj);
		std::vector<Triangle*> vTriangle;
		vTriangle.reserve(numOfEachObj);

		for (size_t i = 0; i < numOfEachObj; ++i)
		{
			vBox.emplace_back(new Box());
			vTriangle.emplace_back(new Triangle());
		}

		std::cout << "call directly: " << benchmark(
			[&vBox, &vTriangle]() {
				for (const auto& obj : vBox)
					obj->Draw();
				for (const auto& obj : vTriangle)
					obj->Draw();
			}, numOfTest) << "\n";

		std::cout << Box::uCounter << " / " << Triangle::uCounter << "\n\n";

		for (auto& obj : vBox)
			delete obj;

		for (auto& obj : vTriangle)
			delete obj;
	}
}
