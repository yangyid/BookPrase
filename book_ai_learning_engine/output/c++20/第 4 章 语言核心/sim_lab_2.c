template<typename T> requires Addable<T>
     void f(T t) {}
     f("abc");