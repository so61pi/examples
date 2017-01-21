#include <iostream>
#include <memory>
#include <utility>


class Data;
using DataPointer = std::shared_ptr<Data>;


class Data {
public:
    template <typename... Args>
    static DataPointer CreateData(Args&&... args) {
        return std::shared_ptr<Data>(new Data(std::forward<Args>(args)...));
    }

private:
    explicit Data(int data) : m_data(data) {}
    Data(Data const&) = default;
    Data(Data&&)      = default;

public:
    int m_data;
};


class CopyOnWrite {
public:
    CopyOnWrite(int data) : m_pData(Data::CreateData(data)) {}

    void ChangeData(int data) {
        CoW();
        m_pData->m_data = data;
    }

    int GetData() const {
        return m_pData->m_data;
    }

    void CoW() {
        if (m_pData.unique()) return;
        m_pData = Data::CreateData(*m_pData);
    }

    long Counter() const {
        return m_pData.use_count();
    }

private:
    std::shared_ptr<Data> m_pData;
};


CopyOnWrite Test() {
    CopyOnWrite a(10);
    auto b = a;
    std::cout << "a.data    = " << a.GetData() << std::endl
              << "b.data    = " << b.GetData() << std::endl
              << "a.counter = " << a.Counter() << std::endl
              << "b.counter = " << b.Counter() << std::endl;

    b.ChangeData(50);
    std::cout << "a.data    = " << a.GetData() << std::endl
              << "b.data    = " << b.GetData() << std::endl
              << "a.counter = " << a.Counter() << std::endl
              << "b.counter = " << b.Counter() << std::endl;

    return a;
}


int main() {
    Test();
}
