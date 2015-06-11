#include <iostream>
#include <memory>
#include <string>
#include <vector>


//
// used from object::model<t>::draw_ to draw a known thing (e.g. int, std::string)
//
template<typename T>
void draw(const T& t, std::ostream& os, int pos) {
    os << std::string(pos, ' ') << t << std::endl;
}


//
// object class that can hold anything that provide a free draw function
//
class object {
private:
    explicit object() = default;

public:
    template<typename T>
    object(const T& val)
        : m_val{ std::make_unique<model<T>>(val) }
    {}

    object(const object& other)
        : m_val{ other.m_val->copy() }
    {}

    object(object&& other)
        : object{}
    {
        swap(other);
    }

    object& operator=(object other) {
        swap(other);
        return *this;
    }

    void swap(object& other) {
        std::swap(m_val, other.m_val);
    }

    friend void draw(const object& o, std::ostream& os, int pos);

private:
    class concept {
    public:
        virtual ~concept() = default;
        virtual std::unique_ptr<concept> copy() = 0;

        // cannot use name draw, see below
        virtual void draw_(std::ostream& os, int pos) const = 0;
    };


    template<typename T>
    class model final : public concept
    {
    public:
        // cannot use m_val{ val }
        //
        // when we have something like this
        //     vector<object> document;
        //     object hell{ document };
        //
        // hell is constructed by a call to object::object<T>(const T& val) [with T = vector<object>],
        // this call news a model<T> which in turn calls model<T>::model(const T& val)
        // at here, a copy constructor should be issued to copy val to m_val
        // but with T = vector<object>, m_val{ val } doesn't call copy constructor
        // of vector [the signature is
        //     vector(const vector<object> other);
        // ],
        // instead it calls initializer-list constructor of vector [the signature is
        //     vector(initializer_list<object> init);
        // ],
        // by constructing a initialzer_list<object> from val by calling
        // object::object<T>(const T& val) [with T = vector<object>]
        // which leads to an infinite recursion
        model(const T& val)
            : concept{},
              m_val(val)
        {}

        ~model() = default;

        virtual std::unique_ptr<concept> copy() override {
            return std::make_unique<model<T>>(m_val);
        }

        virtual void draw_(std::ostream& os, int pos) const override {
            // if use name draw instead of draw_, then this draw call tries to call itself
            // which leads to a compile error (Name Lookup rules)
            draw(m_val, os, pos);
        }

    private:
        T m_val;
    };

private:
    std::unique_ptr<concept> m_val;
};


//
// draw an object
//
void draw(const object& o, std::ostream& os, int pos) {
    o.m_val->draw_(os, pos);
}


//
// draw a vector of object
//
void draw(const std::vector<object>& document, std::ostream& os, int pos) {
    os << std::string(pos, ' ') << "<document>\n";
    for (auto& o : document) {
        draw(o, os, pos + 2);
    }
    os << std::string(pos, ' ') << "</document>\n";
}


//
// create new class and draw function so it can be used with object class
//
class my_class {};

void draw(const my_class&, std::ostream& os, int pos) {
    os << std::string(pos, ' ') << "my_class\n";
}


int main() {
    std::vector<object> document;
    document.emplace_back(1);
    document.emplace_back(std::string{ "abc" });
    document.emplace_back(document);
    document.emplace_back(my_class{});

    draw(document, std::cout, 0);
}
