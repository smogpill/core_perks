// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	template <class T, class Iterator>
	void EraseUnordered(std::vector<T>& v, const Iterator& it)
	{
		if (it + 1 != v.end())
			*it = std::move(v.back());
		v.pop_back();
	}

	template <class T>
	void EraseUnorderedFirst(std::vector<T>& v, const T& element)
	{
		auto it = std::find(v.begin(), v.end(), element);
		if (it != v.end())
			EraseUnordered(v, it);
	}

	template <class T>
	bool Contains(std::vector<T>& v, const T& element)
	{
		return std::find(v.begin(), v.end(), element) != v.end();
	}
}
