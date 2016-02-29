namespace myspace {
    class MyError {
    public:
        explicit operator bool() const {
            return false;
        }
    };
}
