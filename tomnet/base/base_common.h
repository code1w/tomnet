#ifndef BASE_COMMON_H
#define BASE_COMMON_H
#include <cstddef>
#include <cstdint>
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>
namespace tom
{
	namespace net
	{
static const std::string REPORT_RECIPIENT = "qunshuok <qunshuok@gmail.com>";
static const int CALCULATE_ITERATIONS = 15;

struct Key {
    std::uint16_t a;
    std::uint16_t b;
    std::uint16_t c;
    Key() : a(0), b(0), c(0) {
    }
    Key(std::uint16_t a, std::uint16_t b, std::uint16_t c) : a(a), b(b), c(c) {
    }
    bool operator==(const Key& other) const {
        return other.a == a && other.b == b && other.c == c;
    }
    bool operator!=(const Key& other) const {
        return !operator==(other);
    }
    std::string toString() const {
        return "key [" + std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(c) + "]";
    }
};

struct KeyHash1 {
    std::size_t operator()(const Key& key) const {
        const std::size_t prime = 16777619;
        std::size_t hash = 2166136261;
        hash = (hash * prime) ^ key.a;
        hash = (hash * prime) ^ key.b;
        hash = (hash * prime) ^ key.c;
        return hash;
    }
};

struct KeyHash2 {
    std::size_t operator()(const Key& key) const {
        std::size_t hash = 0;
        hash = key.a + (hash << 6) + (hash << 16) - hash;
        hash = key.b + (hash << 6) + (hash << 16) - hash;
        hash = key.c + (hash << 6) + (hash << 16) - hash;
        return hash;
    }
};

}}
#endif // FCMM_TEST_COMMON_H_
