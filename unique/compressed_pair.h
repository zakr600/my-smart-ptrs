#pragma once

#include <type_traits>
#include <utility>

// Me think, why waste time write lot code, when few code do trick.
template <class F, class S, bool = std::is_empty_v<F> && !std::is_final_v<F>,
          bool = std::is_empty_v<S> && !std::is_final_v<S>,
          bool = std::is_base_of_v<S, F> || std::is_base_of_v<F, S>>
class CompressedPair;

template <class F, class S>
class CompressedPair<F, S, true, true, false> : F, S {
public:
    CompressedPair() : F(F()), S(S()){};

    CompressedPair(const F& first, const S& second) : F(first), S(second) {
    }

    CompressedPair(const F& first, S&& second) : F(first), S(std::move(second)) {
    }

    F& GetFirst() {
        return *this;
    }

    const F& GetFirst() const {
        return *this;
    }

    S& GetSecond() {
        return *this;
    };

    const S& GetSecond() const {
        return *this;
    };
};

template <class F, class S>
class CompressedPair<F, S, true, false, false> : F {
public:
    CompressedPair() : F(F()), second_(S()){};

    CompressedPair(const F& first, const S& second) : F(first), second_(second) {
    }

    CompressedPair(const F& first, S&& second) : F(first), second_(std::move(second)) {
    }

    F& GetFirst() {
        return *this;
    }

    const F& GetFirst() const {
        return *this;
    }

    S& GetSecond() {
        return second_;
    };

    const S& GetSecond() const {
        return second_;
    };

private:
    S second_;
};

template <class F, class S>
class CompressedPair<F, S, false, true, false> : S {
public:
    CompressedPair() : first_(F()), S(S()){};

    CompressedPair(const F first, const S second) : first_(first), S(second) {
    }

    F& GetFirst() {
        return first_;
    }

    const F& GetFirst() const {
        return first_;
    }

    S& GetSecond() {
        return static_cast<S&>(*this);
    };

    const S& GetSecond() const {
        return static_cast<S&>(*this);
    };

private:
    F first_;
};

template <class F, class S, bool A, bool B, bool C>
class CompressedPair {
public:
    CompressedPair() : first_(F()), second_(S()){};

    CompressedPair(const F& first, const S& second) : first_(first), second_(second) {
    }

    CompressedPair(const F& first, S&& second) : first_(first), second_(std::move(second)) {
    }

    F& GetFirst() {
        return first_;
    }

    const F& GetFirst() const {
        return first_;
    }

    S& GetSecond() {
        return second_;
    };

    const S& GetSecond() const {
        return second_;
    };

private:
    F first_;
    S second_;
};

template <class F>
class CompressedPair<F, F, true, true, false> : F {
public:
    CompressedPair() : F(F()){};

    CompressedPair(const F& first, const F& second) : F(first) {
    }

    F& GetFirst() {
        return *this;
    }

    const F& GetFirst() const {
        return *this;
    }

    F& GetSecond() {
        return *this;
    }

    const F& GetSecond() const {
        return *this;
    }
};
