#include <cassert>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/any.hpp>


enum class Type {
    String,
    Number,
    Boolean,
    Null,
    Object,
    Array
};


struct Value {
    Value() : type(Type::Null) {}

    Type       type;
    boost::any internal;
};

using Object = std::list<std::pair<std::string, Value>>;
using Array  = std::list<Value>;


bool IsString(Value const& v) {
    return v.type == Type::String;
}

bool IsNumber(Value const& v) {
    return v.type == Type::Number;
}

bool IsBoolean(Value const& v) {
    return v.type == Type::Boolean;
}

bool IsNull(Value const& v) {
    return v.type == Type::Null;
}

bool IsObject(Value const& v) {
    return v.type == Type::Object;
}

bool IsArray(Value const& v) {
    return v.type == Type::Array;
}


Value MakeString(std::string s) {
    Value v;
    v.type = Type::String;
    v.internal = std::move(s);
    return v;
}

Value MakeNumber(double n) {
    Value v;
    v.type = Type::Number;
    v.internal = n;
    return v;
}

Value MakeBoolean(bool b) {
    Value v;
    v.type = Type::Boolean;
    v.internal = b;
    return v;
}

Value MakeNull() {
    return Value();
}

Value MakeObject() {
    Value v;
    v.type = Type::Object;
    v.internal = Object();
    return v;
}

Value MakeArray() {
    Value v;
    v.type = Type::Array;
    v.internal = Array();
    return v;
}


std::string& AsString(Value& v) {
    return boost::any_cast<std::string&>(v.internal);
}

std::string const& AsString(Value const& v) {
    return boost::any_cast<std::string const&>(v.internal);
}

double& AsNumber(Value& v) {
    return boost::any_cast<double&>(v.internal);
}

double const& AsNumber(Value const& v) {
    return boost::any_cast<double const&>(v.internal);
}

bool& AsBoolean(Value& v) {
    return boost::any_cast<bool&>(v.internal);
}

bool const& AsBoolean(Value const& v) {
    return boost::any_cast<bool const&>(v.internal);
}

Object& AsObject(Value& v) {
    return boost::any_cast<Object&>(v.internal);
}

Object const& AsObject(Value const& v) {
    return boost::any_cast<Object const&>(v.internal);
}

Array& AsArray(Value& v) {
    return boost::any_cast<Array&>(v.internal);
}

Array const& AsArray(Value const& v) {
    return boost::any_cast<Array const&>(v.internal);
}


std::string to_string(Object const& o);
std::string to_string(Array const& a);
std::string to_string(Value const& v);

std::string to_string(Object const& o) {
    if (o.empty()) return "{}";

    auto it = o.cbegin();
    std::string result = "{ " + it->first + " : " + to_string(it->second);
    for (it = std::next(it); it != o.cend(); ++it) {
        result += ", " + it->first + " : " + to_string(it->second);
    }
    result += " }";
    return result;
}

std::string to_string(Array const& a) {
    if (a.empty()) return "[]";

    auto it = a.cbegin();
    std::string result = "[ " + to_string(*it);
    for (it = std::next(it); it != a.cend(); ++it) {
        result += ", " + to_string(*it);
    }
    result += " ]";
    return result;
}

std::string to_string(Value const& v) {
    switch (v.type) {
    case Type::String:
        // Well, we have to escape double quotes & backslashes, but just forget it now.
        return '"' + AsString(v) + '"';

    case Type::Number:
        return std::to_string(AsNumber(v));

    case Type::Boolean:
        return AsBoolean(v) ? "true" : "false";

    case Type::Null:
        return "null";

    case Type::Object:
        return to_string(AsObject(v));

    case Type::Array:
        return to_string(AsArray(v));
    }
}


Array::iterator ArrayAppend(Value& a, Value value) {
    Array& array = AsArray(a);
    array.emplace_back(std::move(value));
    return std::prev(array.end());
}

Object::iterator ObjectAddChild(Value& o, std::string key, Value value) {
    Object& object = AsObject(o);
    object.emplace_back(std::move(key), std::move(value));
    return std::prev(object.end());
}


#include <iostream>
void Test() {
    auto o = MakeObject();
    ObjectAddChild(o, "message", MakeString("Hello World!"));
    auto a = MakeArray();
    ArrayAppend(a, MakeNumber(123));
    ArrayAppend(a, MakeNumber(456));
    ArrayAppend(a, MakeNull());
    ArrayAppend(a, MakeString("so61pi"));
    ObjectAddChild(o, "array", a);
    std::cout << to_string(o) << std::endl;
}


int main(int argc, char *argv[]) {
    Test();
}
