template<typename T>
        concept Incrementable = requires(T& x) { x++; ++x; };
        static_assert(Incrementable<int*>);
        static_assert(!Incrementable<std::string>);