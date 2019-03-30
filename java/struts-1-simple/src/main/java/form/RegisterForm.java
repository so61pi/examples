package form;

import javax.servlet.http.HttpServletRequest;

import org.apache.struts.action.ActionErrors;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.action.ActionMessage;
import org.apache.struts.validator.ValidatorForm;

import utils.Status;
import core.User;

public class RegisterForm extends ValidatorForm {
    private String userName = "";
    private String password = "";
    private String passwordConfirmed = "";
    private String email = "";
    private String emailConfirmed = "";
    private String captcha = "";

    //
    //
    //
    @Override
    public ActionErrors validate(ActionMapping mapping,
            HttpServletRequest request) {
        ActionErrors errors = super.validate(mapping, request);
        if (errors.isEmpty()) {
            if (User.isUserExisting(userName) == Status.USER_EXISTING) {
                errors.add("userName", new ActionMessage("errors.userExisting"));
            }

            if (!passwordConfirmed.equals(password)) {
                errors.add("passwordConfirmed", new ActionMessage(
                        "errors.passwordNotMatch"));
            }

            if (User.isEmailExisting(email) == Status.EMAIL_EXISTING) {
                errors.add("email", new ActionMessage("errors.emailExisting"));
            }
            else if (!emailConfirmed.equals(email)) {
                errors.add("emailConfirmed", new ActionMessage(
                        "errors.emailNotMatch"));
            }

            String answer = (String) request.getSession().getAttribute(
                    "CaptchaAnswer");
            if (answer == null || !answer.equals(captcha)) {
                errors.add("captcha", new ActionMessage("error.wrongCaptcha"));
            }
        }
        return errors;
    }

    public boolean empty() {
        return (userName == null || userName.equals(""))
                && (password == null || password.equals(""))
                && (passwordConfirmed == null || passwordConfirmed.equals(""))
                && (email == null || email.equals(""))
                && (emailConfirmed == null || emailConfirmed.equals(""))
                && (captcha == null || captcha.equals(""));
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

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getPasswordConfirmed() {
        return passwordConfirmed;
    }

    public void setPasswordConfirmed(String passwordConfirmed) {
        this.passwordConfirmed = passwordConfirmed;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getEmailConfirmed() {
        return emailConfirmed;
    }

    public void setEmailConfirmed(String emailConfirmed) {
        this.emailConfirmed = emailConfirmed;
    }

    public String getCaptcha() {
        return captcha;
    }

    public void setCaptcha(String captcha) {
        this.captcha = captcha;
    }

}
