#include <offsets.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <type_traits>
#include <stdint.h>
#include <string_view>

using std::string;
using std::stringstream;
using std::cout;

struct type_1 {
    int a;
};

struct type_2 {
    int a;
    int b;
};

struct type_3 {
    int a;
    char b;
    int c;
};

struct type_4 : type_3 {
    uint16_t d;
    uint16_t e;
};

template<typename T, auto MPtr>
static constexpr const size_t offsetof_ptr()
{
    return ((::size_t) & reinterpret_cast<char const volatile&>((((T*)0)->*MPtr)));
}

#if 0  && (defined _MSC_VER && !defined _CRT_USE_BUILTIN_OFFSETOF)
    #ifdef __cplusplus
        #define offsetof(s,m) ((::size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))
    #else
        #define offsetof(s,m) ((size_t)&(((s*)0)->m))
    #endif
#else
    #define offsetof(s,m) __builtin_offsetof(s,m)
#endif

struct concrete {
    int field1;
    int field2;
};

template <>
struct type_offset_info<concrete> {
    constexpr static const char* const fully_qualified_name = "::concrete";

    constexpr static const char* const field_names[] = {
        "field1",
        "field2"
    };

    constexpr static size_t field_types[] = {
        0,
        0
    };

    constexpr static const size_t field_offsets[] = {
        offsetof(concrete, field1),
        offsetof(concrete, field2),
    };
};

struct get_type_info_result {
    enum class  codes {
        success,
        generic_error,
        no_fully_qualified_name,
        size_mismatch
    } code;
    string result = "";
};

template <typename T>
get_type_info_result get_type_offset_info(const string& tabstr = "    ") {
    constexpr const type_offset_info<T> info_instance; // Must be template specialized
    using tinfo = typename ::type_offset_info<T>;

    if (tinfo::fully_qualified_name == nullptr) {
        return {
            get_type_info_result::codes::no_fully_qualified_name
        };
    }

    if (countof(tinfo::field_names) != countof(tinfo::field_types) || countof(tinfo::field_types) != countof(tinfo::field_offsets)) {
        return {
            get_type_info_result::codes::size_mismatch
        };
    }

    stringstream ss;

    ss << tinfo::fully_qualified_name << ": \n";
    for (size_t i = 0; i < countof(tinfo::field_names); ++i) {
        ss << tabstr << tinfo::field_names[i] << ": type(" << tinfo::field_types[i] << ") @ offset " << tinfo::field_offsets[i] << "\n";
    }

    return {
        get_type_info_result::codes::success,
        ss.str()
    };
}

int main() {
    const get_type_info_result& result = get_type_offset_info<concrete>();
    if (result.code != get_type_info_result::codes::success) {
        return 1;
    };

    cout << result.result;

    constexpr const char* const S = "AB" "C";

    constexpr const std::string_view SV = S;

    using t1_a_f = offsets::field_offset<&type_1::a>;
    constexpr const size_t t1_a_f_o = offsetof(type_1, a);

    using t2_a_f = offsets::field_offset<&type_2::a>;
	constexpr const size_t t2_a_f_o = offsetof(type_2, a);
    using t2_b_f = offsets::field_offset<&type_2::b>;

    using t3_a_f = offsets::field_offset<&type_3::a>;
	constexpr const size_t t3_a_f_o = offsetof(type_3, a);
    using t3_b_f = offsets::field_offset<&type_3::b>;
    constexpr const size_t t3_b_f_o = offsetof(type_3, b);
    using t3_c_f = offsets::field_offset<&type_3::c>;
	constexpr const size_t t3_c_f_o = offsetof(type_3, c);

    type_4 o4;
    uint8_t* const o4p = reinterpret_cast<uint8_t*>(&o4);

    using t4_a_f = offsets::field_offset<&type_4::a>;
    uint8_t* const t4_a_p = o4p + t4_a_f::offset;
	constexpr const size_t t4_a_f_o = offsetof(type_4, a);

    using t4_b_f = offsets::field_offset<&type_4::b>;
	uint8_t* const t4_b_p = o4p + t4_b_f::offset;
	constexpr const size_t t4_b_f_o = offsetof(type_4, b);

    using t4_c_f = offsets::field_offset<&type_4::c>;
	uint8_t* const t4_c_p = o4p + t4_c_f::offset;
	constexpr const size_t t4_c_f_o = offsetof(type_4, c);

    using t4_d_f = offsets::field_offset<&type_4::d>;
	uint8_t* const t4_d_p = o4p + t4_d_f::offset;
	constexpr const size_t t4_d_f_o = offsetof(type_4, d);

    return 0;
}
