package form;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import org.apache.struts.action.ActionErrors;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.action.ActionMessage;
import org.apache.struts.validator.ValidatorForm;

import utils.Hasher;
import core.User;

public class ChangePasswordForm extends ValidatorForm {
    private String oldPassword = "";
    private String newPassword = "";
    private String newPasswordConfirmed = "";

    //
    //
    //
    @Override
    public ActionErrors validate(ActionMapping mapping,
            HttpServletRequest request) {
        ActionErrors errors = super.validate(mapping, request);
        if (errors.isEmpty()) {
            HttpSession session = request.getSession();
            int userId = (Integer) session.getAttribute("userId");
            User user = User.getUserById(userId);
            if (!Hasher.md5(oldPassword).equals(user.getHashPass())) {
                errors.add("oldPassword", new ActionMessage(
                        "errors.wrongPassword"));
            }
            else if (!newPasswordConfirmed.equals(newPassword)) {
                errors.add("newPasswordConfirmed", new ActionMessage(
                        "errors.passwordNotMatch"));
            }
        }
        return errors;
    }

    public boolean empty() {
        return (oldPassword == null || oldPassword.equals(""))
                && (newPassword == null || newPassword.equals(""))
                && (newPasswordConfirmed == null || newPasswordConfirmed
                .equals(""));
    }

    //
    // getters & setters
    //
    public String getOldPassword() {
        return oldPassword;
    }

    public void setOldPassword(String oldPassword) {
        this.oldPassword = oldPassword;
    }

    public String getNewPassword() {
        return newPassword;
    }

    public void setNewPassword(String newPassword) {
        this.newPassword = newPassword;
    }

    public String getNewPasswordConfirmed() {
        return newPasswordConfirmed;
    }

    public void setNewPasswordConfirmed(String newPasswordConfirmed) {
        this.newPasswordConfirmed = newPasswordConfirmed;
    }

}
