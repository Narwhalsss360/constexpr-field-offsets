#pragma once
#include <cstddef>

template <typename T>
struct type_offset_info {
    constexpr static const char* const fully_qualified_name = nullptr;
    constexpr static const char* const field_names[0] = {};
    constexpr static const size_t field_types[0] = {};
    constexpr static const size_t field_offsets[0] = {};
};

template <typename T, size_t size>
constexpr const size_t countof(const T (&)[size]) {
    return size;
}

namespace offsets {
	template <typename>
	struct field_type_declaration;

	template <typename T, typename F>
	struct field_type_declaration<F T::*> {
		using struct_type = T;
		using field_type = F;
	};

	static constexpr const char global_source = 0;

	template <auto field>
	struct field_offset {
		using struct_type = typename field_type_declaration<decltype(field)>::struct_type;
#if defined(_WIN32)
		static constexpr const size_t offset = static_cast<size_t>(-1);
#else
		static constexpr const struct_type& source = reinterpret_cast<const struct_type&>(global_source);
		static constexpr const char* const base_addr = &reinterpret_cast<char const&>(source);
		static constexpr const char* field_addr = &reinterpret_cast<char const&>(source.*field);
		static constexpr const size_t offset = field_addr - base_addr;
#endif
	};
}

#if !defined(_WIN32)
#define _offsetof(s, m) ((::size_t)(&reinterpret_cast<const char&>((&reinterpret_cast<const s&>(::offsets::global_source))->m) - &::offsets::global_source))
#define __offsetof(type, field) (::offsets::field_offset<&type::field>::offset)
#endif
