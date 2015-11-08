#include <cassert>
#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/variant.hpp>

#include <windows.h>
#include <atlbase.h>
#include <atlsafe.h>
#include <comdef.h>
#include <shobjidl.h>
#include <wbemidl.h>
#include <winerror.h>

#pragma comment(lib, "wbemuuid.lib")


namespace windows {

    struct result_error : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };


    template <typename T, typename Pred>
    class api_result_checker {
    public:
        api_result_checker(T& result, Pred pred)
            : m_result{result}, m_pred{pred}, m_success{m_pred(m_result)} {}

        auto result() const -> T {
            if (!success()) throw result_error{"api - access error result"};
            return m_result;
        }

        auto success() const -> bool { return m_success; }
        explicit operator bool() const { return success(); }

    private:
        T& m_result;
        Pred m_pred;
        bool m_success;
    };


    class com_result_checker {
    public:
        explicit com_result_checker(HRESULT result)
            : m_result{result}, m_success{SUCCEEDED(m_result)} {}

        auto result() const -> HRESULT {
            if (!success()) throw result_error{"COM - access error result"};
            return m_result;
        }

        auto success() const -> bool { return m_success; }
        explicit operator bool() const { return success(); }

    private:
        HRESULT m_result;
        bool m_success;
    };


    // auxilary functions
    template <typename T, typename Pred>
    auto make_api_result(T result, Pred pred) {
        return api_result_checker<T, Pred>{result, pred};
    }

    template <typename T, typename Pred>
    void check_api_result(T result, Pred pred) {
        make_api_result(result, pred).result();
    }

    auto make_com_result(HRESULT result) { return com_result_checker{result}; }

    void check_com_result(HRESULT result) { make_com_result(result).result(); }

} // namespace windows


auto VariantToString(CComVariant const& variant) -> std::wstring {
    CComVariant out;
    if (SUCCEEDED(
            VariantChangeType(&out, &variant, VARIANT_ALPHABOOL, VT_BSTR))) {
        return out.bstrVal;
    } else {
        switch (variant.vt) {
        case VT_EMPTY:
            return L"[]";

        case VT_NULL:
            return L"[null]";

        case VT_DISPATCH:
            return L"[IDispatch]";

        case VT_ERROR:
            return L"[error]";

        case VT_UNKNOWN:
            return L"[IUnknown]";

        case VT_RECORD:
            return L"[IRecordInfo]";

        case VT_BYREF:
            return L"[byref]";

        case VT_ARRAY:
            return L"[array]";

        default:
            throw std::runtime_error{"COM - unknown variant type"};
        }
    }
}


auto SafeArrayElementToVariant(SAFEARRAY* psa, LONG index) -> CComVariant {
    assert(psa->cDims == 1);
    assert(0 <= index && (index + 0U < psa->rgsabound[0].cElements));

    CComVariant out;
    VARTYPE varType = VT_EMPTY;
    windows::check_com_result(SafeArrayGetVartype(psa, &varType));
    switch (varType) {

#define CASE(VarType, Value)                                                   \
    case VarType:                                                              \
        windows::check_com_result(SafeArrayGetElement(psa, &index, &Value));   \
        out.vt = VarType;                                                      \
        break;

        CASE(VT_I1,     out.cVal)
        CASE(VT_I2,     out.iVal)
        CASE(VT_I4,     out.lVal)
        CASE(VT_INT,    out.intVal)

        CASE(VT_UI1,    out.bVal)
        CASE(VT_UI2,    out.uiVal)
        CASE(VT_UI4,    out.ulVal)
        CASE(VT_UINT,   out.uintVal)

        CASE(VT_R4,     out.fltVal)
        CASE(VT_R8,     out.dblVal)

        CASE(VT_BSTR,   out.bstrVal)

        CASE(VT_BOOL,   out.boolVal)
        CASE(VT_CY,     out.cyVal)
        CASE(VT_DATE,   out.date)
        CASE(VT_DECIMAL,    out.decVal)

        CASE(VT_VARIANT,    out)
        CASE(VT_DISPATCH,   out.pdispVal)
        CASE(VT_UNKNOWN,    out.punkVal)
        CASE(VT_RECORD,     out.pRecInfo)

        CASE(VT_ERROR,  out.scode)

#undef CASE

    default:
        throw std::runtime_error{"COM - invalid variant type in safe array"};
    }

    return out;
}


struct field_t {
    std::wstring name;
    boost::variant<CComVariant, std::vector<CComVariant>> value;
};

using row_t = std::vector<field_t>;
using table_t = std::vector<row_t>;


auto GetFieldValue(CComPtr<IWbemClassObject>& pWCO, BSTR fieldName) -> field_t {
    CComVariant variant;
    windows::check_com_result(pWCO->Get(fieldName, 0, &variant, 0, 0));

    if (variant.vt & VT_ARRAY) {
        if (variant.parray->cDims != 1)
            throw std::runtime_error{"COM - invalid safe array dimensions"};

        auto psa = variant.parray;
        std::vector<CComVariant> array;
        for (auto i = 0U; i < psa->rgsabound[0].cElements; ++i) {
            array.emplace_back(SafeArrayElementToVariant(psa, i));
        }

        return {fieldName, array};
    }

    return {fieldName, variant};
}


auto GetTable(CComPtr<IEnumWbemClassObject>& pEWCO) -> table_t {
    table_t table;

    while (pEWCO) {
        CComPtr<IWbemClassObject> pWCO = nullptr;
        ULONG returnedCount            = 0;

        windows::check_com_result(
            pEWCO->Next(WBEM_INFINITE, 1, &pWCO, &returnedCount));
        if (returnedCount == 0) break;

        SAFEARRAY* fieldNames = nullptr;
        windows::check_com_result(
            pWCO->GetNames(nullptr, WBEM_FLAG_ALWAYS, nullptr, &fieldNames));

        VARTYPE varType{};
        SafeArrayGetVartype(fieldNames, &varType);
        assert(varType == VT_BSTR);
        assert(fieldNames->cDims == 1);

        CComSafeArray<BSTR> propNames;
        propNames.Attach(fieldNames);

        auto numOfFields = propNames.GetCount();
        row_t row;
        row.reserve(numOfFields);
        for (auto i = 0U; i < numOfFields; ++i) {
            row.emplace_back(GetFieldValue(pWCO, propNames.GetAt(i)));
        }

        table.emplace_back(row);
    }

    return table;
}


struct FieldValueVisitor : public boost::static_visitor<void> {
    void operator()(CComVariant variant) const {
        std::wcout << VariantToString(variant);
    }

    void operator()(std::vector<CComVariant> const& array) const {
        std::wcout << L"[";
        for (auto const& e : array) {
            std::wcout << VariantToString(e) << L", ";
        }
        std::wcout << L"]";
    }
};



int main() {
    class tagCoInit {
        HRESULT m_result;

    public:
        tagCoInit() { m_result = CoInitialize(nullptr); }

        ~tagCoInit() {
            if (success()) CoUninitialize();
        }

        bool success() const { return SUCCEEDED(m_result); }
    } CoInit;

    try {
        if (CoInit.success()) {
            CComPtr<IWbemLocator> pWL = nullptr;
            windows::check_com_result(
                pWL.CoCreateInstance(__uuidof(WbemLocator)));

            CComPtr<IWbemServices> pWS = nullptr;
            windows::check_com_result(
                pWL->ConnectServer(bstr_t(L"//./root/cimv2"), nullptr, nullptr,
                                   nullptr, 0, nullptr, nullptr, &pWS));

            windows::check_com_result(
                CoSetProxyBlanket(pWS, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
                                  nullptr, RPC_C_AUTHN_LEVEL_CALL,
                                  RPC_C_IMP_LEVEL_IMPERSONATE, nullptr,
                                  EOAC_NONE));

            CComPtr<IEnumWbemClassObject> pEWCO = nullptr;
            windows::check_com_result(
                pWS->ExecQuery(bstr_t("WQL"),
                               bstr_t("SELECT * FROM Win32_CacheMemory"),
                               WBEM_FLAG_FORWARD_ONLY
                                   | WBEM_FLAG_RETURN_IMMEDIATELY,
                               nullptr, &pEWCO));

            for (auto const& row : GetTable(pEWCO)) {
                for (auto const& field : row) {
                    std::wcout << field.name << L" : ";
                    boost::apply_visitor(FieldValueVisitor{}, field.value);
                    std::wcout << L"\n";
                }
                std::wcout << L"\n";
            }
        }
    } catch (std::exception const& e) {
        std::cout << "exception : " << e.what() << '\n';
    }
}
