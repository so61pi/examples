package form;

import javax.servlet.http.HttpServletRequest;

import org.apache.struts.action.ActionErrors;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.action.ActionMessage;
import org.apache.struts.validator.ValidatorForm;

import utils.Status;
import core.User;

public class ForgotPasswordForm extends ValidatorForm {
    private String email = "";
    private String captcha = "";

    //
    //
    //
    @Override
    public ActionErrors validate(ActionMapping mapping,
            HttpServletRequest request) {
        ActionErrors errors = super.validate(mapping, request);
        if (errors.isEmpty()) {
            if (User.isEmailExisting(email) == Status.EMAIL_NOT_EXISTING) {
                errors.add("email",
                        new ActionMessage("errors.emailNotExisting"));
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
        return (email == null || email.equals(""))
                && (captcha == null || captcha.equals(""));
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
