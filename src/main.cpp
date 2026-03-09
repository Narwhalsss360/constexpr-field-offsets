#include <offsets.hpp>
#include <string>
#include <sstream>
#include <iostream>

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
    enum result_types {
        success,
        error
    } type;
    union {
        string s;
    };
};

template <typename T>
string get_type_offset_info(const string& tabstr = "    ") {
    constexpr const type_offset_info<T> info_instance; // Must be template specialized
    using tinfo = typename ::type_offset_info<T>;

    if (tinfo::fully_qualified_name == nullptr) {
        return string();
    }

    if (countof(tinfo::field_names) != countof(tinfo::field_types) || countof(tinfo::field_types) != countof(tinfo::field_offsets)) {
        return string();
    }

    stringstream ss;

    ss << tinfo::fully_qualified_name << ": \n";
    for (size_t i = 0; i < countof(tinfo::field_names); ++i) {
        ss << tabstr << tinfo::field_names[i] << ": type(" << tinfo::field_types[i] << ") @ offset " << tinfo::field_offsets[i] << "\n";
    }

    return ss.str();
}

int main() {
    cout << get_type_offset_info<concrete>();
    return 0;
}
