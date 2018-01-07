#include <algorithm>
#include <cassert>
#include <cstddef>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <typeinfo>
#include <utility>
#include <vector>

#include <boost/any.hpp>
#include <boost/type_index.hpp>


//
//
//
template <typename Class, typename FSig>
struct member_function_pointer;

template <typename Class, typename Ret, typename... Params>
struct member_function_pointer<Class, Ret(Params...)> {
    using type        = Ret (Class::*)(Params...);
    using result_type = Ret;
    using params_type = std::tuple<Params...>;
};


//
//
//
template <typename T>
std::string get_type_name() {
    return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}


struct rtypeinfo {
    rtypeinfo(std::type_info const& ti, std::string const& n,
              std::string const& sn = "")
        : typeinfo{ti}, name{n}, sname{sn} {}

    std::type_info const& typeinfo;
    std::string const name;
    std::string const sname;
};


auto operator==(rtypeinfo const& lhs, rtypeinfo const& rhs) -> bool {
    return lhs.typeinfo == rhs.typeinfo && lhs.name == rhs.name;
}


template <typename T>
auto make_rti(std::string const& sname = "") {
    return rtypeinfo{typeid(T), get_type_name<T>(), sname};
}


template <typename... T>
auto make_params_sig() -> std::vector<rtypeinfo> {
    std::vector<rtypeinfo> result;
    result.reserve(sizeof...(T));
    std::initializer_list<int>{
        (result.emplace_back(typeid(T), get_type_name<T>()), 0)...};
    return result;
}


//
//
//
class creator {
public:
    virtual auto params() const -> std::vector<rtypeinfo> const& = 0;
    virtual auto create(std::vector<boost::any>& params) const
        -> boost::any = 0;
    virtual ~creator() = default;
};


template <typename Class, typename... Params>
class dcreator : public creator {
public:
    using type = Class;

    dcreator() : m_params_sig{make_params_sig<Params...>()} {}

    virtual auto params() const -> std::vector<rtypeinfo> const& override {
        return m_params_sig;
    }

    virtual auto create(std::vector<boost::any>& params) const
        -> boost::any override {
        using tuple_t = std::tuple<Params...>;
        return create_helper<tuple_t,
                             std::make_index_sequence<std::tuple_size<tuple_t>::
                                                          value>>{}(params);
    }

private:
    template <typename Tuple, typename Indices>
    struct create_helper;

    template <typename... T, std::size_t... I>
    struct create_helper<std::tuple<T...>, std::index_sequence<I...>> {
        static_assert(sizeof...(T) == sizeof...(I), "error");

        auto operator()(std::vector<boost::any>& params) const -> type {
            return type{boost::any_cast<T>(params[I])...};
        }
    };

    // for empty parameters list ctor
    template <std::size_t... I>
    struct create_helper<std::tuple<void>, std::index_sequence<I...>> {
        static_assert(1 == sizeof...(I), "error");

        auto operator()(std::vector<boost::any>& params) const -> type {
            return type{};
        }
    };

private:
    std::vector<rtypeinfo> m_params_sig;
};


//
//
//
class caller {
public:
    virtual auto name() const -> std::string const& = 0;
    virtual auto params() const -> std::vector<rtypeinfo> const& = 0;
    virtual auto call(boost::any& obj, std::vector<boost::any>& params) const
        -> boost::any = 0;
    virtual ~caller() = default;
};


template <typename Class, typename FSig>
class dcaller : public caller {
    //
    template <typename>
    struct make_params_sig_helper;

    template <typename Ret, typename... Params>
    struct make_params_sig_helper<Ret(Params...)> {
        auto operator()() const { return make_params_sig<Params...>(); }
    };

    //
    template <typename Ret, typename Params, typename Indices>
    struct call_helper;

    // has return type
    template <typename Ret, typename... T, std::size_t... I>
    struct call_helper<Ret, std::tuple<T...>, std::index_sequence<I...>> {
        static_assert(sizeof...(T) == sizeof...(I), "error");

        template <typename PSig>
        auto operator()(Class& obj, PSig pmf,
                        std::vector<boost::any>& params) const -> boost::any {
            return (obj.*pmf)(boost::any_cast<T>(params[I])...);
        }
    };

    // doesn't have return type
    template <typename... T, std::size_t... I>
    struct call_helper<void, std::tuple<T...>, std::index_sequence<I...>> {
        static_assert(sizeof...(T) == sizeof...(I), "error");

        template <typename PSig>
        auto operator()(Class& obj, PSig pmf,
                        std::vector<boost::any>& params) const -> boost::any {
            (obj.*pmf)(boost::any_cast<T>(params[I])...);
            return boost::any{};
        }
    };


public:
    using pmfsig_type = typename member_function_pointer<Class, FSig>::type;
    using params_type =
        typename member_function_pointer<Class, FSig>::params_type;
    using result_type =
        typename member_function_pointer<Class, FSig>::result_type;

    dcaller(std::string const& name, pmfsig_type pmf)
        : m_name{name},
          m_pmf{pmf},
          m_params_sig{make_params_sig_helper<FSig>{}()} {}

    virtual auto name() const -> std::string const& override { return m_name; }

    virtual auto params() const -> std::vector<rtypeinfo> const& override {
        return m_params_sig;
    }

    virtual auto call(boost::any& obj, std::vector<boost::any>& params) const
        -> boost::any override {
        return call_helper<result_type, params_type,
                           std::
                               make_index_sequence<std::
                                                       tuple_size<params_type>::
                                                           value>>{}(
            boost::any_cast<Class&>(obj), m_pmf, params);
    }

private:
    std::string m_name;
    pmfsig_type m_pmf;
    std::vector<rtypeinfo> m_params_sig;
};


//
//
//
class registry {
public:
    virtual auto rti() const -> rtypeinfo const& = 0;
    virtual auto creators() const
        -> std::vector<std::unique_ptr<creator>> const& = 0;
    virtual auto callers() const
        -> std::vector<std::unique_ptr<caller>> const& = 0;
    virtual ~registry() = default;
};


template <typename Class>
class dregistry : public registry {
public:
    dregistry(std::string const& sname = "") : m_rti{make_rti<Class>(sname)} {}

    template <typename... Params>
    auto addcreator() -> dregistry& {
        m_creators.emplace_back(std::make_unique<dcreator<Class, Params...>>());
        return *this;
    }


    template <typename Ret, typename... Params>
    auto addcaller(std::string const& fname, Ret (Class::*pmf)(Params...))
        -> dregistry& {
        m_callers.emplace_back(
            std::make_unique<dcaller<Class, Ret(Params...)>>(fname, pmf));
        return *this;
    }


    virtual auto rti() const -> rtypeinfo const& override { return m_rti; }

    virtual auto creators() const
        -> std::vector<std::unique_ptr<creator>> const& override {
        return m_creators;
    }

    virtual auto callers() const
        -> std::vector<std::unique_ptr<caller>> const& override {
        return m_callers;
    }

private:
    rtypeinfo m_rti;
    std::vector<std::unique_ptr<creator>> m_creators;
    std::vector<std::unique_ptr<caller>> m_callers;
};


//
//
//
class meta {
public:
    static auto get() -> std::vector<std::unique_ptr<meta>>& {
        static std::vector<std::unique_ptr<meta>> s;
        return s;
    }


    static auto types() -> std::vector<rtypeinfo>& {
        static std::vector<rtypeinfo> r;
        return r;
    }


public:
    meta(registry const& reg) : m_reg{reg} {}

    auto rti() const -> rtypeinfo const& { return m_reg.rti(); }

    auto create(std::vector<boost::any>& params,
                std::vector<rtypeinfo>& rparaminfos) const -> boost::any {
        for (auto const& c : m_reg.creators()) {
            if (c->params() == rparaminfos) {
                return c->create(params);
            }
        }

        throw std::runtime_error{"cannot find creator"};
    }

    auto call(boost::any& obj, std::string const& fname,
              std::vector<boost::any>& params,
              std::vector<rtypeinfo>& rparaminfos) const -> boost::any {
        for (auto const& c : m_reg.callers()) {
            if (c->name() == fname && c->params() == rparaminfos) {
                return c->call(obj, params);
            }
        }

        throw std::runtime_error{"cannot find caller"};
    }

private:
    registry const& m_reg;
};


auto get(std::string const& name) -> meta const& {
    for (auto const& m : meta::get()) {
        if (m->rti().name == name) {
            return *m;
        }
    }

    for (auto const& m : meta::get()) {
        if (m->rti().sname == name) {
            return *m;
        }
    }

    throw std::runtime_error{"cannot find class"};
}


template <typename... T>
auto create(meta const& m, T... params) -> boost::any {
    std::vector<boost::any> vparams;
    vparams.reserve(sizeof...(T));
    std::vector<rtypeinfo> rtis;
    rtis.reserve(sizeof...(T));

    std::initializer_list<int>{(vparams.emplace_back(params), 0)...};
    std::initializer_list<int>{(rtis.emplace_back(make_rti<T>()), 0)...};

    return m.create(vparams, rtis);
}


// for ctor without any parameters
auto create(meta const& m) -> boost::any {
    std::vector<boost::any> vparams;
    vparams.emplace_back(boost::any{});
    std::vector<rtypeinfo> rtis;
    rtis.emplace_back(make_rti<void>());
    return m.create(vparams, rtis);
}


template <typename... T>
auto call(meta const& m, boost::any& obj, std::string const& fname, T... params)
    -> boost::any {
    std::vector<boost::any> vparams;
    vparams.reserve(sizeof...(T));
    std::vector<rtypeinfo> rtis;
    rtis.reserve(sizeof...(T));

    std::initializer_list<int>{(vparams.emplace_back(params), 0)...};
    std::initializer_list<int>{(rtis.emplace_back(make_rti<T>()), 0)...};

    return m.call(obj, fname, vparams, rtis);
}


// helper macros
#define REGISTER_BEGIN(CLASS, SECONDARY)                                       \
    struct CLASS##_meta_initor_t {                                             \
        dregistry<CLASS> dreg{SECONDARY};                                      \
        CLASS##_meta_initor_t() {

#define REGISTER_CREATOR(...)                                                  \
        dreg.addcreator<__VA_ARGS__>();

#define REGISTER_METHOD(NAME, ADDRESS)                                         \
        dreg.addcaller(NAME, ADDRESS);

#define REGISTER_END(CLASS)                                                    \
        meta::get().emplace_back(std::make_unique<meta>(dreg));                \
        }                                                                      \
    } CLASS##_meta_initor;

#define REGISTER_TYPE_NAME(TYPE, NAME)                                         \
    class TYPE##_type_name_t {                                                 \
        TYPE##_type_name_t() {                                                 \
            meta::types().emplace_back(typeid(TYPE), get_type_name<TYPE>(),    \
                                       NAME);                                  \
        }                                                                      \
    } TYPE##_type_name;


//
//
//
class test_t {
public:
    test_t() = default;
    test_t(int value) : m_value{value} {}

    void show(int align) {
        std::cout << std::string(align, ' ') << m_value << "\n";
    }

    void show() { show(0); }

    int get() { return m_value; }

private:
    int m_value = 10;
};


REGISTER_BEGIN(test_t, "class ::test_t")
    REGISTER_CREATOR(void)
    REGISTER_CREATOR(int)
    REGISTER_METHOD("show", static_cast<void(test_t::*)(int)>(&test_t::show))
    REGISTER_METHOD("show", static_cast<void(test_t::*)()>(&test_t::show))
    REGISTER_METHOD("get", &test_t::get)
REGISTER_END(test_t)


int main() {
    try {
        auto const& m = get("class ::test_t");
        {
            auto o = create(m);
            call(m, o, "show");
            call(m, o, "show", 4);
            auto r = call(m, o, "get");
            std::cout << boost::any_cast<int>(r) << "\n";
        }

        {
            auto o = create(m, 2);
            call(m, o, "show");
            call(m, o, "show", 2);
            auto r = call(m, o, "get");
            std::cout << boost::any_cast<int>(r) << "\n";
        }
    } catch (std::exception const& e) {
        std::cout << e.what();
    }
}
