#pragma once

#include <type_traits>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

/**
 * @brief prints an IP address represented by an integral type
 * @tparam integral Integral type (int, short, long, etc.)
 * @param ip IP address in host byte order
 * @details this function takes an integral value, interprets it as an IP address in host byte order,
 *          and prints it in dotted decimal notation (e.g., 127.0.0.1).
 */
template <typename Integral, enable_if_t<is_integral_v<remove_reference_t<Integral>>, bool> = true>
void print_ip(Integral&& ip) {
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&ip);
    for (size_t i = sizeof(Integral); i > 0; --i) {
        cout << static_cast<int>(bytes[i - 1]) << ((i == 1) ? '\n' : '.');
    }
}

/**
 * @brief prints an IP address represented by a string
 * @tparam S string type (must be std::string)
 * @param ip IP address as a string
 * @details this function simply prints the string as-is, followed by a newline.
 *          It's a specialization for string representation of IP addresses.
 */
template <typename S, enable_if_t<is_same_v<remove_reference_t<S>, string>, bool> = true>
void print_ip(S&& ip) {
    cout << ip << endl;
}

/**
 * @brief prints an IP address represented by a container
 * @tparam container Type of container (vector, list, array, etc.)
 * @param container container with IP address parts
 * @details this function prints each element of the container separated by dots.
 *          The container must support begin() and end() iterators.
 *          Example: vector{192, 168, 1, 1} prints as "192.168.1.1"
 */
template <typename Container>
auto print_ip(const Container& container) -> void_t<decltype(begin(container))> 
{
    auto it = begin(container);
    const auto cont_end = end(container);

    if (it != cont_end) {
        cout << *it;
        ++it;
    }

    for (; it != cont_end; ++it) {
        cout << "." << *it;
    }
    cout << endl;
}

template <typename... Args>
constexpr bool are_all_same_v = std::conjunction_v<
    std::is_same<Args, std::tuple_element_t<0, std::tuple<Args...>>>...
>;

/**
 * @brief prints an IP address represented by a tuple
 * @tparam tupleArgs Types of tuple elements
 * @param tuple tuple containing IP address parts
 * @details this function prints each element of the tuple separated by dots.
 *          All tuple elements must be of the same type (checked at compile-time).
 *          Example: tuple{192, 168, 1, 1} prints as "192.168.1.1"
 * @throws static_assert If tuple elements have different types
 */
template <typename... TupleArgs>
void print_ip(const std::tuple<TupleArgs...>& tuple) {
    static_assert(are_all_same_v<TupleArgs...>, "All tuple elements MUST BE the same type!");

    std::apply([](const auto&... args) {
        size_t n = 0;
        ((std::cout << args << (++n != sizeof...(TupleArgs) ? "." : "\n")), ...);
    }, tuple);
}
