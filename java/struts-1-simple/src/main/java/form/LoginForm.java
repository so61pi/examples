package form;

import javax.servlet.http.HttpServletRequest;

import org.apache.struts.action.ActionErrors;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.action.ActionMessage;
import org.apache.struts.validator.ValidatorForm;

import utils.Status;
import core.User;

public class LoginForm extends ValidatorForm {
    private String userName = "";
    private String password = "";

    //
    //
    //
    @Override
    public ActionErrors validate(ActionMapping mapping,
            HttpServletRequest request) {
        ActionErrors errors = super.validate(mapping, request);
        if (errors.isEmpty()) {
            Status s = User.authenticate(userName, password);
            if (s == Status.USER_NOT_EXISTING) {
                errors.add("userName", new ActionMessage(
                        "errors.userNotExisting"));
            }
            else if (s == Status.PASSWORD_NOT_MATCH) {
                errors.add("password",
                        new ActionMessage("errors.wrongPassword"));
            }
        }
        return errors;
    }

    public boolean empty() {
        return (userName == null || userName.equals(""))
                && (password == null || password.equals(""));
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

}
