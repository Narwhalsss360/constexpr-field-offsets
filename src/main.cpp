#include <offsets.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <type_traits>

using std::string;
using std::stringstream;
using std::cout;

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
    return 0;
}
