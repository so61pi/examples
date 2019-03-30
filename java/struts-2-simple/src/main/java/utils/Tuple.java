package utils;

public class Tuple<X, Y, Z, T, U> {
    private final X x;
    private final Y y;
    private final Z z;
    private final T t;
    private final U u;

    public Tuple(X x, Y y, Z z, T t, U u) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.t = t;
        this.u = u;
    }

    public X getX() {
        return x;
    }

    public Y getY() {
        return y;
    }

    public Z getZ() {
        return z;
    }

    public T getT() {
        return t;
    }

    public U getU() {
        return u;
    }
}
