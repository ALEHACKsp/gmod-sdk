#include "netvars.hpp"

#include "../sdk/createinterface.hpp"
#include "../sdk/vfunc.hpp"

namespace Netvars
{
	std::unordered_map<unsigned int, std::unordered_map<unsigned int, std::ptrdiff_t>> database;

	void Store(const char* group, detail::RecvTable* table, std::ptrdiff_t offset = 0)
	{
		for (size_t i = 0; i < table->count; ++i)
		{
			auto prop = &table->props[i];

			if (!prop || std::isdigit(*prop->name))
				continue;

			auto child = prop->table;

			if (child && child->count)
				Store(group, child, prop->offset);

			hash32_t hash = hash::fnv1a_32(group), name = hash::fnv1a_32(prop->name);

			if (database[hash][name] <= 0 && 0 <= prop->type && prop->type <= 4)
				database[hash][name] = static_cast<std::ptrdiff_t>(prop->offset) + offset;
		}
	}

	__forceinline void Init() {
		auto chl = CreateInterface<void>("client.dll", "VClient");
		auto classes = CallVFunction<GetAllClasses_t>(chl, 8)(chl);

		for (; classes; classes = classes->next)
			Store(classes->table->table, classes->table);
	}

	__forceinline int GetOffset(hash32_t table, hash32_t prop) {
		return database[table][prop];
	}
}