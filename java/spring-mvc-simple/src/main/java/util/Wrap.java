package util;

public class Wrap<T> {
    T o;

    public Wrap(T o) {
        this.o = o;
    }

    public T get() {
        return o;
    }

    public void set(T o) {
        this.o = o;
    }
}
