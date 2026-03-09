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
