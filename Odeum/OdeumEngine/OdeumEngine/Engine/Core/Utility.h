#ifndef UTILITY_H
#define UTILITY_H

namespace Utility
{
	template <typename T> bool isAligned(T value_, size_t alignment_)
	{
		return 0 == ((size_t)value_ & (alignment_ - 1));
	}

	template <typename T> T Align(T value_, size_t alignment_)
	{
		return (T)(((value_ + alignment_ - 1) / alignment_) * alignment_);
	}
}

#endif