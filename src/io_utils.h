#pragma once

#include <cmath>
#include <iomanip>
#include <istream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace bank_detail {

inline void requireNonnegative(double value, const char* field) {
    if (!std::isfinite(value) || value < 0.0) {
        throw std::invalid_argument(std::string(field) + " must be finite and nonnegative");
    }
}

inline void requireYears(double years) {
    if (!std::isfinite(years) || years <= 0.0) {
        throw std::invalid_argument("years must be finite and positive");
    }
}

inline void requireFiniteMetric(double value) {
    if (!std::isfinite(value)) {
        throw std::invalid_argument("metric is outside the finite range");
    }
}

inline bool consume(std::istream& in, const char* expected) {
    in >> std::ws;
    for (const char* p = expected; *p != '\0'; ++p) {
        char ch = '\0';
        if (!in.get(ch) || ch != *p) {
            in.setstate(std::ios::failbit);
            return false;
        }
    }
    return true;
}

inline bool labeled(std::istream& in, char first) {
    in >> std::ws;
    return in && in.peek() == first;
}

inline bool readValue(std::istream& in, const char* label, double& value) {
    return consume(in, label) && static_cast<bool>(in >> value);
}

inline bool readValue(std::istream& in, const char* label, int& value) {
    return consume(in, label) && static_cast<bool>(in >> value);
}

inline bool readMetric(std::istream& in, double expected) {
    double value = 0.0;
    if (!readValue(in, "metric=", value)) {
        return false;
    }
    if (!std::isfinite(value) || std::abs(value - expected) >= 1e-6) {
        in.setstate(std::ios::failbit);
        return false;
    }
    return true;
}

inline std::ostringstream output() {
    std::ostringstream out;
    out << std::setprecision(std::numeric_limits<double>::max_digits10);
    return out;
}

}
