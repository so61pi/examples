package utils;

import java.security.Key;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

public class Crypto {
    private static Key aesKey;

    static {
        // 128 bit key
        String key = "Bar12345Bar12345";

        // Create key and cipher
        aesKey = new SecretKeySpec(key.getBytes(), "AES");

    }

    public static byte[] encrypt(String plainText) {
        try {
            Cipher cipher = Cipher.getInstance("AES");
            cipher.init(Cipher.ENCRYPT_MODE, aesKey);
            byte[] encrypted = cipher.doFinal(plainText.getBytes());
            return encrypted;
        }
        catch (Exception e) {
            ExceptionHandler.handle(e);
        }
        return null;
    }

    public static String decrypt(byte[] cipherText) {
        try {
            Cipher cipher = Cipher.getInstance("AES");
            cipher.init(Cipher.DECRYPT_MODE, aesKey);
            String decrypted = new String(cipher.doFinal(cipherText));
            return decrypted;
        }
        catch (Exception e) {
            ExceptionHandler.handle(e);
        }
        return "";
    }

}
