class DeleterBase {
public:
    virtual void dispose() = 0;
    virtual ~DeleterBase() = default;
};


template<typename T>
class DefaultDeleter : public DeleterBase {
public:
    DefaultDeleter(T *p) :
        m_p{ p }
    {}

    virtual void dispose() override {
        delete m_p;
    }

private:
    T *m_p;
};


template<typename T, typename Deleter>
class CustomDeleter : public DeleterBase {
public:
    CustomDeleter(T *p, Deleter deleter) :
        m_p{ p },
        m_deleter{ deleter }
    {}

    virtual void dispose() override {
        m_deleter(m_p);
    }

private:
    T *m_p;
    Deleter m_deleter;
};


class Pointer {
public:
    template<typename T>
    Pointer(T *p) :
        m_deleter{ new DefaultDeleter<T>{ p } }
        //Pointer(p, [](T *p) { delete p; })
    {}
    
    template<typename T, typename Deleter>
    Pointer(T *p, Deleter deleter) :
        m_deleter{ new CustomDeleter<T, Deleter>{ p, deleter } }
    {}
    
    ~Pointer() {
        m_deleter->dispose();
        delete m_deleter;
    }

private:
    DeleterBase *m_deleter;
};


int main() {
    Pointer p{ new int };
}
