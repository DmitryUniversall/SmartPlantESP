#pragma once


namespace Types {
    template<typename T1, typename T2>
    struct Pair {
        T1 first;
        T2 second;

        Pair(const T1& f, const T2& s) : first(f), second(s) {}
    };

    template<typename T1, typename T2, typename T3>
    struct Triple {
        T1 first;
        T2 second;
        T3 third;

        Triple(const T1& f, const T2& s, const T3& t) : first(f), second(s), third(t) {}
    };
}
