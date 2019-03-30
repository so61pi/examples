package util;

import java.util.HashMap;
import java.util.Map;

public class ClientStatus {
    private String status;
    private Map<String, String> errors = new HashMap<String, String>();

    //
    //
    //
    public ClientStatus() {
    }

    public ClientStatus(String status) {
        this.status = status;
    }

    public void addError(String key, String error) {
        errors.put(key, error);
    }

    //
    //
    //
    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public Map<String, String> getErrors() {
        return errors;
    }

    public void setErrors(Map<String, String> errors) {
        this.errors = errors;
    }
}
