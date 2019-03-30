package util;

import java.util.Date;

public class Console {
    static public void log() {
        System.out.println();
    }

    static public void log(Object o) {
        System.out.println(o);
    }

    static public void logTime() {
        System.out.println(new Date());
    }

    static public void logTime(Object o) {
        System.out.println((new Date()).toString() + "\n" + o);
    }
}
