#ifndef HRESULT_CHECK_H
#define HRESULT_CHECK_H


#include <stdexcept>
#include <string>

#include <Windows.h>


class HRESULTCheck final {
public:
    HRESULTCheck() = default;

    HRESULTCheck(HRESULT hr) :
        m_hr(hr)
    {
        check();
    }


    void operator=(HRESULT hr) {
        m_hr = hr;
        check();
    }


    HRESULT get() const {
        return m_hr;
    }


private:
    void check() const {
        if (FAILED(m_hr)) {
            throw std::runtime_error("COM error : " + std::to_string(m_hr));
        }
    }


private:
    HRESULT m_hr{};
};


#endif // HRESULT_CHECK_H
