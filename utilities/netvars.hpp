#pragma once
#include "utilities.hpp"
#include <unordered_map>

namespace Netvars
{
	namespace detail
	{
		struct RecvProp;
		struct DVariant
		{
			union
			{
				float floating;
				long integer;
				char* string;
				void* data;
				float vector[3];
				int64_t int64;
			};

			int type;
		};

		struct CRecvProxyData
		{
			const RecvProp* prop;
			DVariant value;
			int element, object;
		};

		struct RecvTable
		{
			RecvProp* props;
			int       count;
			void* decoder;
			char* table;
			bool      initialized;
			bool      listed;
		};

		using recv_var_proxy_t = void(__cdecl*)(const CRecvProxyData*, void*, void*);

		struct RecvProp
		{
			char* name;
			int type;
			int flags;
			int size;
			bool listed;
			const void* data;
			RecvProp* array;
			void* length;
			recv_var_proxy_t proxy;
			void* table_proxy;
			RecvTable* table;
			int offset, stride, count;
			const char* parent;
		};

		struct ClientClass
		{
			void* create, * event;
			const char* name;
			RecvTable* table;
			ClientClass* next;
			int id;
		};
	}
	
	using GetAllClasses_t = detail::ClientClass * (__thiscall*)(void*);

	extern std::unordered_map<unsigned int, std::unordered_map<unsigned int, std::ptrdiff_t>> database;
	extern __forceinline void Init();
	extern __forceinline int GetOffset(hash32_t table, hash32_t prop);
}
