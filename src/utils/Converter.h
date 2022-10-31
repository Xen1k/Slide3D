#pragma once

namespace converter
{
	template<class T>
	T& toReference(T* pointer) {
		return *pointer;
	}
}