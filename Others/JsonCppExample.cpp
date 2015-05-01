#include <iostream>
#include <string>

#include "json/json.h"


int main() {
    //
    // json null value
    //
    // Json::Value null_value(Json::nullValue); also works
    Json::Value null_value;


    //
    // json object
    //
    Json::Value object(Json::objectValue);
    object["name"] = "Some name";
    object["age"] = 10;
    object["married"] = true;
    object["address"] = null_value;

    // get number of values
    std::cout << object.size() << "\n";

    // check if a member exist or not
    if (object.isMember("name"))
        std::cout << "Name : " << object["name"] << "\n";

    // check type of member
    if (object["age"].isNumeric())
        std::cout << "age is a number.\n";

    // check if object can be converted to another type
    if (!object.isConvertibleTo(Json::stringValue))
        std::cout << "Object cannot be converted to string.\n";

    // loop through all names
    for (auto& name : object.getMemberNames())
        std::cout << name << "\n";

    // loop through all values
    for (auto& val : object)
        std::cout << val << "\n";

    // remove a member
    object.removeMember("age");

    // print the whole object out
    std::cout << object << "\n\n";


    //
    // json array
    //
    Json::Value array(Json::arrayValue);
    array[0u] = "a";
    array[1u] = 10;
    array[2u] = false;
    array[4u] = 3.14; // array[3u] is automatically set to null
    array[5u] = null_value; 
    array.append(object);

    // check if 'array' is an array
    std::cout << array.isArray() << "\n";

    // check if array is empty
    std::cout << array.empty() << "\n";

    // get number of elements in array
    std::cout << "Number of elements : " << array.size() << "\n";

    // loop through all elements
    for (auto& val : array)
        std::cout << val << "\n";

    // check if an index is valid
    if (array.isValidIndex(1)) {
        // remove member by index
        Json::Value removed;
        array.removeIndex(1, &removed);
    }
    
    std::cout << array << "\n\n";


    //
    // write json to string
    //
    Json::FastWriter fast_writer;
    auto json_str = fast_writer.write(object);
    std::cout << json_str << "\n\n";

    // create styled json string
    Json::StyledWriter styled_writer;
    auto styled_json_str = styled_writer.write(array);
    std::cout << styled_json_str << "\n\n";


    //
    // parse string to json
    //
    Json::Reader reader;
    Json::Value root;
    reader.parse(json_str, root);
    if (!reader.good())
        std::cout << reader.getFormattedErrorMessages() << "\n";
    else
        std::cout << root << "\n\n";
}
