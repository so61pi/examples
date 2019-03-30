package actions;

import java.util.Map;

import org.apache.commons.validator.routines.EmailValidator;
import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.InterceptorRef;
import org.apache.struts2.convention.annotation.InterceptorRefs;
import org.apache.struts2.convention.annotation.Result;

import utils.Status;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

import core.User;

@InterceptorRefs({ @InterceptorRef("loggedinStack") })
@Action(value = "register", results = {
        @Result(name = "success", location = "success.jsp"),
        @Result(name = "error", location = "error.jsp") })
public class RegisterAction extends ActionSupport {
    private String userName;
    private String pass;
    private String confirmedPass;
    private String email;
    private String confirmedEmail;
    private String captcha;

    @Override
    public String execute() {
        if (allFieldNull()) {
            return INPUT;
        }

        Map request = (Map) ActionContext.getContext().get("request");
        if (User.register(userName, pass, email) == Status.SUCCESS) {

            request.put("reason", "An activating link is sent to your email!");
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
        if (allFieldNull()) {
            setFieldErrors(null);
            return;
        }

        // userName
        if (userName == null || userName.length() < 6) {
            addFieldError("userName",
                    "User name must be at least 6 characters.");
        }
        else if (User.isUserExisting(userName) == Status.USER_EXISTING) {
            addFieldError("userName", "This user is already used.");
        }

        // pass
        if (pass == null || pass.length() < 6) {
            addFieldError("pass", "Password must be at least 6 characters.");
        }
        // confirmedPass
        else if (confirmedPass == null || !confirmedPass.equals(pass)) {
            addFieldError("confirmedPass", "Password does NOT match.");
        }

        // email
        if (email == null || email.equals("")) {
            addFieldError("email", "Please fill in your email.");
        }
        else if (!EmailValidator.getInstance().isValid(email)) {
            addFieldError("email", "Your email is not valid.");
        }
        else if (User.isEmailExisting(email) == Status.EMAIL_EXISTING) {
            addFieldError("email", "This email is already used.");
        }
        // confirmedEmail
        else if (confirmedEmail == null || !confirmedEmail.equals(email)) {
            addFieldError("confirmedEmail", "Email does NOT match.");
        }

        Map session = (Map) ActionContext.getContext().get("session");
        String answer = (String) session.get("CaptchaAnswer");
        if (captcha == null || !captcha.equals(answer)) {
            addFieldError("captcha", "Wrong captcha.");
        }
    }

    //
    // stupid method
    //
    private boolean allFieldNull() {
        return userName == null && pass == null && confirmedPass == null
                && email == null && confirmedEmail == null && captcha == null;
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

    public String getConfirmedPass() {
        return confirmedPass;
    }

    public void setConfirmedPass(String confirmedPass) {
        this.confirmedPass = confirmedPass;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getConfirmedEmail() {
        return confirmedEmail;
    }

    public void setConfirmedEmail(String confirmedEmail) {
        this.confirmedEmail = confirmedEmail;
    }

    public String getCaptcha() {
        return captcha;
    }

    public void setCaptcha(String captcha) {
        this.captcha = captcha;
    }

}
