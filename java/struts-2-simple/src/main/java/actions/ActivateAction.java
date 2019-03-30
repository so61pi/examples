package actions;

import java.util.Map;

import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.Result;

import utils.Crypto;
import utils.ExceptionHandler;
import utils.Hex;
import utils.Status;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

import core.User;

@Action(value = "activate", results = {
        @Result(name = "success", location = "success.jsp"),
        @Result(name = "error", location = "error.jsp") })
public class ActivateAction extends ActionSupport {
    private String code;

    @Override
    public String execute() {
        Map request = (Map) ActionContext.getContext().get("request");
        try {
            if (code != null) {
                String userName = Crypto.decrypt(Hex.toByteArray(code));
                if (User.isUserExisting(userName) == Status.USER_EXISTING) {
                    User.activateUser(User.getUserByName(userName));

                    request.put("reason", "Your account is fully activated.");
                    return SUCCESS;
                }
            }
        }
        catch (Exception e) {
            ExceptionHandler.handle(e);
        }
        request.put("reason", "Please re-check the URL.");
        return ERROR;
    }

    //
    // getters & setters
    //
    public String getCode() {
        return code;
    }

    public void setCode(String code) {
        this.code = code;
    }
}
