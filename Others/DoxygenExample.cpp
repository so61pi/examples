/**
@file
@brief
    Brief file description.

@details
    Detail description.

@author so61pi
*/

#include <string>

/**
@brief
    Test class.

@invariant
    Nothing.
*/
class Test {
public:
    /**
    @brief
        Brief description.

    @param      val     Parameter description.
    */
    void show(int val);


    /**
    @brief
        Brief description.

    @details
        Detail description for set_cursor() function.

    @tparam     AType       Template parameter description.
    @tparam     BType       Template parameter description.

    @param          row     Parameter description.
    @param[in]      col     Parameter description.
        @parblock
            First paragraph.

            Second paragraph.
                @arg First item.
                @arg Second item.
                @arg Third item.

            Third paragraph.
        @endparblock
    @param[out]     str     Parameter description.

    @return
        Nothing.

    @exception  noexcept
    @exception  std::exception  Throw exception.

    @note
        Notes.

    @pre
        Pre-conditions.

    @post
        Post-conditions.

    @warning
        Warnings.

    @par User-defined paragraph
        <b>Bold text</b>. <em>Italic text</em>. <tt>Type font</tt>.

    @par User-defined paragraph
    @parblock
        Something
            @arg <b>Bold text</b>.
            @arg <em>Italic text</em>.
            @arg <tt>Type font</tt>.
    @endparblock

    @code
        // example
        try {
            Test test;
            std::string str;
            test.get(1, '2', str);
        }
        catch (std::exception& e) {
            std::cout << e.what() << "\n";
        }
    @endcode
    */
    void get(int row, int col, std::string& str);


public:
    /**
    @brief  Brief description.
    */
    enum struct ScopedEnum {
        Yes,  //!< Brief description.

        /**
        @brief  Brief description.
        @details
            Detail description.
                @arg First item.
                @arg Second item.
                @arg Third item.
        */
        No
    };

    /**
    @brief  Brief description.
    */
    enum Enum {
        False,  //!< Brief description.
        True    //!< Brief description.
    };


protected:
    int m_member; //!< Brief data member description.

    /**
    @brief  Complex data member description.
    @details
        Detail description.
    */
    int m_complex_member;
};