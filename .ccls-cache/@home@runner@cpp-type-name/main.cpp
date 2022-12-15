#include <iostream>
#include <ostream>
#include <type_traits>
#include <vector>
#include <string>
#include <map>

//************************
//* FUNDAMENTIAL TYPES
//************************

template<typename... Ts>
struct type_name
{
    friend std::ostream& operator<<(std::ostream& os, type_name)
    {
        return os;
    }
};

#define FUNDAMENTIAL_TYPE_NAME(type)                                        \
template<>                                                                  \
struct type_name<type>                                                      \
{                                                                           \
    friend std::ostream& operator<<(std::ostream& os, type_name)            \
    {                                                                       \
        os << #type;                                                        \
        return os;                                                          \
    }                                                                       \
}

FUNDAMENTIAL_TYPE_NAME(void);
#if __cplusplus >= 201402L // C++14
FUNDAMENTIAL_TYPE_NAME(std::nullptr_t);
#endif //__cplusplus >= 201402L
FUNDAMENTIAL_TYPE_NAME(bool);
FUNDAMENTIAL_TYPE_NAME(char);
FUNDAMENTIAL_TYPE_NAME(signed char);
FUNDAMENTIAL_TYPE_NAME(unsigned char);
FUNDAMENTIAL_TYPE_NAME(short int);
FUNDAMENTIAL_TYPE_NAME(int);
FUNDAMENTIAL_TYPE_NAME(long int);
FUNDAMENTIAL_TYPE_NAME(long long int);
FUNDAMENTIAL_TYPE_NAME(unsigned short int);
FUNDAMENTIAL_TYPE_NAME(unsigned int);
FUNDAMENTIAL_TYPE_NAME(unsigned long int);
FUNDAMENTIAL_TYPE_NAME(unsigned long long int);
FUNDAMENTIAL_TYPE_NAME(float);
FUNDAMENTIAL_TYPE_NAME(double);
FUNDAMENTIAL_TYPE_NAME(long double);

//************************
//* COMPOUND TYPES
//************************

#define COMPOUND_TYPE_NAME_DELIMITED(delimiter, type_modifier)              \
template<typename T>                                                        \
struct type_name<T type_modifier>                                           \
{                                                                           \
    friend std::ostream& operator<<(std::ostream& os, type_name)            \
    {                                                                       \
        os << type_name<T>();                                               \
        os << delimiter << #type_modifier;                                  \
        return os;                                                          \
    }                                                                       \
}

#define COMPOUND_TYPE_NAME(type_modifier)                                   \
    COMPOUND_TYPE_NAME_DELIMITED("", type_modifier)

// ARRAYS
COMPOUND_TYPE_NAME([]);

template<typename T, std::size_t N>
struct type_name<T[N]> : type_name<T[]> {};

// POINTER, REF and CV
COMPOUND_TYPE_NAME(*);
COMPOUND_TYPE_NAME(&);
COMPOUND_TYPE_NAME(&&);
COMPOUND_TYPE_NAME_DELIMITED(" ", const);
COMPOUND_TYPE_NAME_DELIMITED(" ", volatile);
COMPOUND_TYPE_NAME_DELIMITED(" ", const volatile);

// ARGUMENTS
template<typename Arg, typename... Args>
struct type_name<Arg, Args...>
{
    friend std::ostream& operator<<(std::ostream& os, type_name)
    {
        os << type_name<Arg>() << ", " << type_name<Args...>();
        return os;
    }
};

// FUNCTION
template<typename R, typename... Args>
struct type_name<R(Args...)>
{
    friend std::ostream& operator<<(std::ostream& os, type_name)
    {
        os << type_name<R>();
        os << "(" << type_name<Args...>() << ")";
        return os;
    }
};

#define FUNCTION_CVR_TYPE_NAME(cvr)                                         \
template<typename R, typename... Args>                                      \
struct type_name<R(Args...) cvr>                                            \
{                                                                           \
    friend std::ostream& operator<<(std::ostream& os, type_name)            \
    {                                                                       \
        os << type_name<R(Args...)>();                                      \
        os << " " << #cvr;                                                  \
        return os;                                                          \
    }                                                                       \
}

FUNCTION_CVR_TYPE_NAME(const);
FUNCTION_CVR_TYPE_NAME(volatile);
FUNCTION_CVR_TYPE_NAME(const volatile);
FUNCTION_CVR_TYPE_NAME(&);
FUNCTION_CVR_TYPE_NAME(const &);
FUNCTION_CVR_TYPE_NAME(volatile &);
FUNCTION_CVR_TYPE_NAME(const volatile &);
FUNCTION_CVR_TYPE_NAME(&&);
FUNCTION_CVR_TYPE_NAME(const &&);
FUNCTION_CVR_TYPE_NAME(volatile &&);
FUNCTION_CVR_TYPE_NAME(const volatile &&);

// POINTER TO FUNCTION, REF TO FUNCTION
#define FUNCTION_PR_TYPE_NAME(pr)                                           \
template<typename R, typename... Args>                                      \
struct type_name<R(pr)(Args...)>                                            \
{                                                                           \
    friend std::ostream& operator<<(std::ostream& os, type_name)            \
    {                                                                       \
        os << type_name<R>();                                               \
        os << "(" << #pr << ")";                                            \
        os << "(" << type_name<Args...>() << ")";                           \
        return os;                                                          \
    }                                                                       \
}

FUNCTION_PR_TYPE_NAME(*);    // function pointer
FUNCTION_PR_TYPE_NAME(* const);
FUNCTION_PR_TYPE_NAME(* volatile);
FUNCTION_PR_TYPE_NAME(* const volatile);
FUNCTION_PR_TYPE_NAME(&);    // function lvalue reference
FUNCTION_PR_TYPE_NAME(&&);    // function rvalue reference

// function with mix of plain and variardic arguments

#define FUNCTION_MIX_CVR_TYPE_NAME(cvr)                                     \
template<typename R, typename... Args>                                      \
struct type_name<R(Args..., ...) cvr>                                       \
{                                                                           \
    friend std::ostream& operator<<(std::ostream& os, type_name)            \
    {                                                                       \
        os << type_name<R>();                                               \
        os << "(" << type_name<Args...>() <<  ", ...)";                     \
        if (!std::string(#cvr).empty()) { os << " "; }                      \
        os << #cvr;                                                         \
        return os;                                                          \
    }                                                                       \
}

FUNCTION_MIX_CVR_TYPE_NAME();
FUNCTION_MIX_CVR_TYPE_NAME(const);
FUNCTION_MIX_CVR_TYPE_NAME(volatile);
FUNCTION_MIX_CVR_TYPE_NAME(const volatile);
FUNCTION_MIX_CVR_TYPE_NAME(&);
FUNCTION_MIX_CVR_TYPE_NAME(const &);
FUNCTION_MIX_CVR_TYPE_NAME(volatile &);
FUNCTION_MIX_CVR_TYPE_NAME(const volatile &);
FUNCTION_MIX_CVR_TYPE_NAME(&&);
FUNCTION_MIX_CVR_TYPE_NAME(const &&);
FUNCTION_MIX_CVR_TYPE_NAME(volatile &&);
FUNCTION_MIX_CVR_TYPE_NAME(const volatile &&);

#define FUNCTION_PR_MIX_TYPE_NAME(pr)                                       \
template<typename R, typename... Args>                                      \
struct type_name<R(pr)(Args..., ...)>                                       \
{                                                                           \
    friend std::ostream& operator<<(std::ostream& os, type_name)            \
    {                                                                       \
        os << type_name<R>();                                               \
        os << "(" << #pr << ")";                                            \
        os << "(" << type_name<Args...>() <<  ", ...)";                     \
        return os;                                                          \
    }                                                                       \
}

FUNCTION_PR_MIX_TYPE_NAME(*);    // function pointer
FUNCTION_PR_MIX_TYPE_NAME(* const);
FUNCTION_PR_MIX_TYPE_NAME(* volatile);
FUNCTION_PR_MIX_TYPE_NAME(* const volatile);
FUNCTION_PR_MIX_TYPE_NAME(&);    // function lvalue reference
FUNCTION_PR_MIX_TYPE_NAME(&&);    // function rvalue reference

// function with variardic arguments
#define FUNCTION_VAR_TYPE_NAME(cvr)                                         \
template<typename R>                                                        \
struct type_name<R(...) cvr>                                                \
{                                                                           \
    friend std::ostream& operator<<(std::ostream& os, type_name)            \
    {                                                                       \
        os << type_name<R>();                                               \
        os << "(...)";                                                      \
        if (!std::string(#cvr).empty()) { os << " "; }                      \
        os << #cvr;                                                         \
        return os;                                                          \
    }                                                                       \
}

FUNCTION_VAR_TYPE_NAME();
FUNCTION_VAR_TYPE_NAME(const);
FUNCTION_VAR_TYPE_NAME(volatile);
FUNCTION_VAR_TYPE_NAME(const volatile);
FUNCTION_VAR_TYPE_NAME(&);
FUNCTION_VAR_TYPE_NAME(const &);
FUNCTION_VAR_TYPE_NAME(volatile &);
FUNCTION_VAR_TYPE_NAME(const volatile &);
FUNCTION_VAR_TYPE_NAME(&&);
FUNCTION_VAR_TYPE_NAME(const &&);
FUNCTION_VAR_TYPE_NAME(volatile &&);
FUNCTION_VAR_TYPE_NAME(const volatile &&);

// function with variardic arguments
#define FUNCTION_PR_VAR_TYPE_NAME(pr)                                       \
template<typename R>                                                        \
struct type_name<R(pr)(...)>                                                \
{                                                                           \
    friend std::ostream& operator<<(std::ostream& os, type_name)            \
    {                                                                       \
        os << type_name<R>();                                               \
        os << "(" << #pr << ")";                                            \
        os << "(...)";                                                      \
        return os;                                                          \
    }                                                                       \
}

FUNCTION_PR_VAR_TYPE_NAME(*);    // function pointer
FUNCTION_PR_VAR_TYPE_NAME(* const);
FUNCTION_PR_VAR_TYPE_NAME(* volatile);
FUNCTION_PR_VAR_TYPE_NAME(* const volatile);
FUNCTION_PR_VAR_TYPE_NAME(&);    // function lvalue reference
FUNCTION_PR_VAR_TYPE_NAME(&&);    // function rvalue reference

// function pointer to member
#define FUNCTION_CLASS_PTR_TYPE_NAME(p, cvr)                                \
template<typename R, typename C, typename... Args>                          \
struct type_name<R(C:: p)(Args...) cvr>                                     \
{                                                                           \
    friend std::ostream& operator<<(std::ostream& os, type_name)            \
    {                                                                       \
        os << type_name<R>();                                               \
        os << "(" << type_name<C>() << "::" << #p << ")";                   \
        os << "(" << type_name<Args...>() << ")";                           \
        if (!std::string(#cvr).empty()) { os << " "; }                      \
        os << #cvr;                                                         \
        return os;                                                          \
    }                                                                       \
}

FUNCTION_CLASS_PTR_TYPE_NAME(*,);
FUNCTION_CLASS_PTR_TYPE_NAME(*, const);
FUNCTION_CLASS_PTR_TYPE_NAME(*, volatile);
FUNCTION_CLASS_PTR_TYPE_NAME(*, const volatile);
FUNCTION_CLASS_PTR_TYPE_NAME(*, &);
FUNCTION_CLASS_PTR_TYPE_NAME(*, const &);
FUNCTION_CLASS_PTR_TYPE_NAME(*, volatile &);
FUNCTION_CLASS_PTR_TYPE_NAME(*, const volatile &);
FUNCTION_CLASS_PTR_TYPE_NAME(*, &&);
FUNCTION_CLASS_PTR_TYPE_NAME(*, const &&);
FUNCTION_CLASS_PTR_TYPE_NAME(*, volatile &&);
FUNCTION_CLASS_PTR_TYPE_NAME(*, const volatile &&);

FUNCTION_CLASS_PTR_TYPE_NAME(* const,);
FUNCTION_CLASS_PTR_TYPE_NAME(* const, const);
FUNCTION_CLASS_PTR_TYPE_NAME(* const, volatile);
FUNCTION_CLASS_PTR_TYPE_NAME(* const, const volatile);
FUNCTION_CLASS_PTR_TYPE_NAME(* const, &);
FUNCTION_CLASS_PTR_TYPE_NAME(* const, const &);
FUNCTION_CLASS_PTR_TYPE_NAME(* const, volatile &);
FUNCTION_CLASS_PTR_TYPE_NAME(* const, const volatile &);
FUNCTION_CLASS_PTR_TYPE_NAME(* const, &&);
FUNCTION_CLASS_PTR_TYPE_NAME(* const, const &&);
FUNCTION_CLASS_PTR_TYPE_NAME(* const, volatile &&);
FUNCTION_CLASS_PTR_TYPE_NAME(* const, const volatile &&);

FUNCTION_CLASS_PTR_TYPE_NAME(* volatile,);
FUNCTION_CLASS_PTR_TYPE_NAME(* volatile, const);
FUNCTION_CLASS_PTR_TYPE_NAME(* volatile, volatile);
FUNCTION_CLASS_PTR_TYPE_NAME(* volatile, const volatile);
FUNCTION_CLASS_PTR_TYPE_NAME(* volatile, &);
FUNCTION_CLASS_PTR_TYPE_NAME(* volatile, const &);
FUNCTION_CLASS_PTR_TYPE_NAME(* volatile, volatile &);
FUNCTION_CLASS_PTR_TYPE_NAME(* volatile, const volatile &);
FUNCTION_CLASS_PTR_TYPE_NAME(* volatile, &&);
FUNCTION_CLASS_PTR_TYPE_NAME(* volatile, const &&);
FUNCTION_CLASS_PTR_TYPE_NAME(* volatile, volatile &&);
FUNCTION_CLASS_PTR_TYPE_NAME(* volatile, const volatile &&);

FUNCTION_CLASS_PTR_TYPE_NAME(* const volatile,);
FUNCTION_CLASS_PTR_TYPE_NAME(* const volatile, const);
FUNCTION_CLASS_PTR_TYPE_NAME(* const volatile, volatile);
FUNCTION_CLASS_PTR_TYPE_NAME(* const volatile, const volatile);
FUNCTION_CLASS_PTR_TYPE_NAME(* const volatile, &);
FUNCTION_CLASS_PTR_TYPE_NAME(* const volatile, const &);
FUNCTION_CLASS_PTR_TYPE_NAME(* const volatile, volatile &);
FUNCTION_CLASS_PTR_TYPE_NAME(* const volatile, const volatile &);
FUNCTION_CLASS_PTR_TYPE_NAME(* const volatile, &&);
FUNCTION_CLASS_PTR_TYPE_NAME(* const volatile, const &&);
FUNCTION_CLASS_PTR_TYPE_NAME(* const volatile, volatile &&);
FUNCTION_CLASS_PTR_TYPE_NAME(* const volatile, const volatile &&);

// function pointer to member, mixed args
#define FUNCTION_CLASS_PTR_MIX_TYPE_NAME(p, cvr)                            \
template<typename R, typename C, typename... Args>                          \
struct type_name<R(C:: p)(Args..., ...) cvr>                                \
{                                                                           \
    friend std::ostream& operator<<(std::ostream& os, type_name)            \
    {                                                                       \
        os << type_name<R>();                                               \
        os << "(" << type_name<C>() << "::" << #p << ")";                   \
        os << "(" << type_name<Args...>()  <<  ", ...)";                    \
        if (!std::string(#cvr).empty()) { os << " "; }                      \
        os << #cvr;                                                         \
        return os;                                                          \
    }                                                                       \
}

FUNCTION_CLASS_PTR_MIX_TYPE_NAME(*,);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(*, const);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(*, volatile);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(*, const volatile);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(*, &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(*, const &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(*, volatile &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(*, const volatile &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(*, &&);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(*, const &&);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(*, volatile &&);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(*, const volatile &&);

FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const,);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const, const);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const, volatile);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const, const volatile);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const, &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const, const &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const, volatile &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const, const volatile &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const, &&);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const, const &&);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const, volatile &&);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const, const volatile &&);

FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* volatile,);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* volatile, const);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* volatile, volatile);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* volatile, const volatile);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* volatile, &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* volatile, const &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* volatile, volatile &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* volatile, const volatile &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* volatile, &&);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* volatile, const &&);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* volatile, volatile &&);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* volatile, const volatile &&);

FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const volatile,);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const volatile, const);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const volatile, volatile);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const volatile, const volatile);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const volatile, &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const volatile, const &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const volatile, volatile &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const volatile, const volatile &);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const volatile, &&);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const volatile, const &&);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const volatile, volatile &&);
FUNCTION_CLASS_PTR_MIX_TYPE_NAME(* const volatile, const volatile &&);

// function pointer to member, variadic args
#define FUNCTION_CLASS_PTR_VAR_TYPE_NAME(p, cvr)                            \
template<typename R, typename C>                                            \
struct type_name<R(C:: p)(...) cvr>                                         \
{                                                                           \
    friend std::ostream& operator<<(std::ostream& os, type_name)            \
    {                                                                       \
        os << type_name<R>();                                               \
        os << "(" << type_name<C>() << "::" << #p << ")";                   \
        os << "(...)";                                                      \
        if (!std::string(#cvr).empty()) { os << " "; }                      \
        os << #cvr;                                                         \
        return os;                                                          \
    }                                                                       \
}

FUNCTION_CLASS_PTR_VAR_TYPE_NAME(*,);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(*, const);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(*, volatile);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(*, const volatile);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(*, &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(*, const &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(*, volatile &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(*, const volatile &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(*, &&);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(*, const &&);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(*, volatile &&);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(*, const volatile &&);

FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const,);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const, const);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const, volatile);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const, const volatile);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const, &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const, const &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const, volatile &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const, const volatile &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const, &&);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const, const &&);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const, volatile &&);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const, const volatile &&);

FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* volatile,);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* volatile, const);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* volatile, volatile);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* volatile, const volatile);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* volatile, &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* volatile, const &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* volatile, volatile &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* volatile, const volatile &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* volatile, &&);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* volatile, const &&);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* volatile, volatile &&);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* volatile, const volatile &&);

FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const volatile,);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const volatile, const);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const volatile, volatile);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const volatile, const volatile);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const volatile, &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const volatile, const &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const volatile, volatile &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const volatile, const volatile &);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const volatile, &&);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const volatile, const &&);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const volatile, volatile &&);
FUNCTION_CLASS_PTR_VAR_TYPE_NAME(* const volatile, const volatile &&);


/* SFINAE attempt -- not working
#if __cplusplus < 201402L // C++11 or before
namespace std
{
    template< bool B, class T = void >
    using enable_if_t = typename enable_if<B, T>::type;
}
#endif //__cplusplus < 201402L
template <typename F>
using enable_if_function = std::enable_if_t<std::is_function<F>::value>;
template <typename F, typename = enable_if_function<F> >
struct type_name<F const> : type_name<F> {};
*/

// template class
template <template <typename...> class C, typename... Args>
struct type_name< C<Args...> >
{
    friend std::ostream& operator<<(std::ostream& os, type_name)
    {
        os << "template";
        os << "<" << type_name<Args...>() << ">";

        return os;
    }
};

// class
template <typename C>
struct type_name<C>
{
    friend std::ostream& operator<<(std::ostream& os, type_name)
    {
        if (std::is_union<C>::value) {
            os << "union";
        }
        else if (std::is_enum<C>::value)
        {
            os << "enum";
        }
        else
        {
            os << "class";
        }

        return os;
    }
};

// pointer to member
template <typename T, typename C>
struct type_name<T C::*>
{
    friend std::ostream& operator<<(std::ostream& os, type_name)
    {
        os << type_name<T>() << " class::*";

        return os;
    }
};


template<typename T>
void print()
{
    std::cout << type_name<T>() << std::endl;
}

int f(int) { return 0; };

enum e {};

// C++17
enum struct e2 : int {};
enum byte : unsigned char {};

enum struct e3 {};

enum smallenum: int16_t
{
    a,
    b,
    c
};

template <typename T>
struct t_s {};

struct fn {
    int operator()(int) { return 0; };
};

int main()
{
    print<int>();
    print<const int>();
    print<int volatile>();
    print<const int volatile>();
    print<int[]>();
    print<int[][4]>();
    print<int&>();
    print<decltype(std::move(5))>();
    print<int*>();
    print<const int *>();
    print<const int * const>();
    print<int const * const>();
    print<int * const>();
    print<int(int, char, const int *, char**)>();
    print<int(...)>();
    print<int(const char *, ...)>();
    print<int(const char *, float, ...)>();
    print<int(*)(int)>();
    print<int(int) const>();
    print<int(int) volatile>();
    print<int(int) const volatile>();
    //http://codexpert.ro/blog/2014/10/17/c-gems-ref-qualifiers/
    print<int(int) const &>();
    print<int(int) volatile &>();
    print<int(int) const volatile &>();
    print<int(int) const &&>();
    print<int(int) volatile &&>();
    print<int(int) const volatile &&>();
    print<int(int) const noexcept>();
    print<int(...) const>();
    print<int(void)>();
    print<void(void)>();
    print<void*(void*, void*)>();
    print<decltype(&printf)>();
    auto l = [](){};
    print<decltype(l)>();
    print<union u>();
    print<struct c>();
    struct s{};
    print<int const s::* const volatile>();
    print<void (s::*)(int) const volatile &&>();
    print<void (s::*)(...) const volatile &&>();
    print<void (s::* const volatile)(int, char, ...) const volatile &&>();
    print<int(*)(int)>();
    print<int(&)(int)>();
    print<int(&&)(int)>();
    print<int(* const volatile)(int)>();
    print<int(*)(...)>();
    print<int(*)(int, ...)>();
    print<ptrdiff_t>();
    print<std::vector<int> >();
    print<std::vector<std::vector<int> > >();
    print<std::vector<std::string> >();
    print< t_s<int> >();
    print< t_s<s> >();
    print<e>();
    print<e2>();
    print<e3>();
    print<byte>();
    print<smallenum>();
    print<
        const volatile std::map<std::string, int>& 
        (t_s<s>::* const volatile) (const volatile std::vector<int>&&,
                                    char**, int[][5], const int * const, 
                                    int, ...)
        const volatile &&
    >();

    auto it = std::map<std::string, int>::const_reverse_iterator();
    print<decltype(it)>();

    auto lambda = []() mutable -> int { return 0; };
    print<decltype(lambda)>();

    print< std::map<int, decltype(lambda)> >();

    return 0;
}