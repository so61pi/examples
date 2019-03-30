package utils;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class Json {
    public static String readJsonFromInputStream(InputStream inputStream) {
        String json = "";

        try {
            int c;
            while ((c = inputStream.read()) != -1) {
                json += (char) c;
            }
        }
        catch (IOException e) {
            ExceptionHandler.handle(e);
        }

        return json;
    }

    public static void writeJsonToOutputStream(OutputStream outputStream,
            String s) {
        try {
            outputStream.write(s.getBytes());
        }
        catch (IOException e) {
            ExceptionHandler.handle(e);
        }
    }
}
