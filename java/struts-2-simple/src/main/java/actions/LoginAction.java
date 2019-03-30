package actions;

import java.util.HashMap;
import java.util.Map;

import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.InterceptorRef;
import org.apache.struts2.convention.annotation.InterceptorRefs;
import org.apache.struts2.convention.annotation.Result;

import utils.Status;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

import core.User;

@InterceptorRefs({ @InterceptorRef("loggedinStack") })
@Action(value = "login", results = {
        @Result(name = "success", type = "redirect", location = "/home"),
        @Result(name = "error", location = "error.jsp") })
public class LoginAction extends ActionSupport {
    private String userName;
    private String pass;

    @Override
    public String execute() {
        if (allFieldNull()) {
            return INPUT;
        }

        Map session = (Map) ActionContext.getContext().get("session");
        Map request = (Map) ActionContext.getContext().get("request");

        User user = User.getUserByName(userName);
        if (user != null) {
            Map<Integer, Integer> cart = new HashMap<Integer, Integer>();
            session.put("cart", cart);

            session.put("user", user);
            return SUCCESS;
        }
        else {
            request.put("reason", "Internal Error!");
            return ERROR;
        }
    }

    //
    // validation
    //
    @Override
    public void validate() {
        // if all fields are null, that means the page is loaded the first time,
        // so don't do any validation stuff
        if (allFieldNull()) {
            setFieldErrors(null);
            return;
        }

        // userName
        if (userName == null || userName.length() < 6) {
            addFieldError("userName", "Your user name is NOT valid.");
        }
        else if (pass == null) {
            addFieldError("pass", "Please fill in your password.");
        }
        else {
            Status s = User.authenticate(userName, pass);
            if (s == Status.USER_NOT_EXISTING) {
                addFieldError("userName", "This user name does NOT exist!");
            }
            else if (s == Status.PASSWORD_NOT_MATCH) {
                addFieldError("pass", "Wrong password.");
            }
        }
    }

    //
    // stupid method
    //
    private boolean allFieldNull() {
        return userName == null && pass == null;
    }

    //
    // getters & setters
    //
    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getPass() {
        return pass;
    }

    public void setPass(String pass) {
        this.pass = pass;
    }

}
