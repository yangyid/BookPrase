int count = 0;
     auto lazy = v | rv::filter([&](int i){ ++count; return i%2==0; });
     // 此时 count == 0
     for(auto x : lazy) { /* use x */ }
     // 此时 count == 遍历的元素数