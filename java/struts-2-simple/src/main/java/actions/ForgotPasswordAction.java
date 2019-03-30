package actions;

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
@Action(value = "forgot-password", results = {
        @Result(name = "success", location = "success.jsp"),
        @Result(name = "error", location = "error.jsp") })
public class ForgotPasswordAction extends ActionSupport {
    private String email;
    private String captcha;

    @Override
    public String execute() {
        if (allFieldNull()) {
            return INPUT;
        }

        Map request = (Map) ActionContext.getContext().get("request");

        try {
            User user = User.getUserByEmail(email);
            User.forgotPass(user);
            request.put("reason", "A new password has been sent to your email!");
            return SUCCESS;
        }
        catch (Exception e) {
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

        // email
        if (email == null || email.equals("")) {
            addFieldError("email", "Please fill in your email.");
        }
        else if (User.isEmailExisting(email) == Status.EMAIL_NOT_EXISTING) {
            addFieldError("email", "This email does NOT exist.");
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
        return email == null && captcha == null;
    }

    //
    // getters & setters
    //
    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getCaptcha() {
        return captcha;
    }

    public void setCaptcha(String captcha) {
        this.captcha = captcha;
    }

}
