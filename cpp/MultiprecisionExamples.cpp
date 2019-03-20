#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>


int main() {
    //
    // check multiprecision/doc/html/boost_multiprecision/tut/ints/cpp_int.html
    // for checked & unchecked types
    //

    // integer number type
    using boost::multiprecision::cpp_int;   // unlimited precision
    using boost::multiprecision::int1024_t;
    using boost::multiprecision::uint1024_t;

    using boost::multiprecision::checked_cpp_int;
    using boost::multiprecision::checked_int1024_t;
    using boost::multiprecision::checked_uint1024_t;

    // rational number type
    using boost::multiprecision::cpp_rational;

    // floating point number type
    using boost::multiprecision::cpp_dec_float_100;
    using cpp_dec_float_200 = boost::multiprecision::number < boost::multiprecision::cpp_dec_float < 200 > > ;


    //
    // real code below
    //

    // calculate factorial of 50
    cpp_int factorial{ 1 };
    // cpp_int factorial{ "1" }; also works
    for (cpp_int i{ 1 }; i <= 50; ++i)
        factorial *= i;

    std::cout << "Factorial of 50 is\n\t" << factorial << "\n\n";


    cpp_rational rational{ cpp_int{ 321 }, cpp_int{ 123 } };
    // cpp_rational rational{ "321/123" }; also works
    rational *= 10;
    // print 50 numbers of rational
    std::cout << rational << " is \n\t"
        << rational.convert_to<cpp_dec_float_100>().str(50) << "\n\n";


    // calculate square root of 2
    std::cout << "sqrt(2)\n\t"
        << std::setprecision(std::numeric_limits<cpp_dec_float_200>::digits10)
        << boost::multiprecision::sqrt(cpp_dec_float_200{ 2 });
}
