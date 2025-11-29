// CorePerks (https://github.com/smogpill/CorePerks)
// SPDX-FileCopyrightText: 2025 Jounayd ID SALAH
// SPDX-License-Identifier: MIT
#pragma once

namespace cp
{
	class BinaryOutputStream;
	class ResourceLoader;

	class Resource
	{
	public:
		virtual ~Resource() {}

		virtual bool OnLoad(ResourceLoader& loader) = 0 { return true; }
		virtual bool OnDependenciesLoaded() { return true; }
		virtual void OnStore(cp::BinaryOutputStream& stream) const {}
	};
}
