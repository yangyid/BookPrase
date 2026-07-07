template<typename T>
        concept StrictTotalOrder = std::equality_comparable<T> && requires(const T& a, const T& b) {
            { a < b } -> std::convertible_to<bool>;
        };