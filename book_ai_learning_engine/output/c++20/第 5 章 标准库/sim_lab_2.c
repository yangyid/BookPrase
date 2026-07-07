std::vector<int> v = {1,2,3};
     std::span<int> sp{v};
     // breakpoint here, print &v[0] and sp.data() → same
     v.clear(); // or let it go out of scope
     // sp[0] = 5; // crash