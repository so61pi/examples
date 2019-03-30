package actions;

import java.util.Map;

import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.InterceptorRef;
import org.apache.struts2.convention.annotation.InterceptorRefs;
import org.apache.struts2.convention.annotation.Result;

import utils.Hasher;

import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;

import core.User;

@InterceptorRefs({ @InterceptorRef("notLoggedinStack") })
@Action(value = "change-password", results = {
        @Result(name = "success", location = "success.jsp"),
        @Result(name = "error", location = "error.jsp") })
public class ChangePasswordAction extends ActionSupport {
    private String oldPass;
    private String newPass;
    private String confirmedNewPass;

    @Override
    public String execute() {
        if (allFieldNull()) {
            return INPUT;
        }

        Map session = (Map) ActionContext.getContext().get("session");
        Map request = (Map) ActionContext.getContext().get("request");

        try {
            User.changePass((User) session.get("user"), newPass);

            request.put("reason", "Your password is changed successfully.");
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

        if (oldPass == null) {
            addFieldError("oldPass", "Please fill in your current password.");
        }
        else {
            Map<String, Object> session = ActionContext.getContext()
                    .getSession();
            int userId = (Integer) session.get("userId");
            User user = User.getUserById(userId);
            if (!Hasher.md5(oldPass).equals(user.getHashPass())) {
                addFieldError("oldPass", "Wrong password.");
            }
        }

        if (newPass == null || newPass.length() < 6) {
            addFieldError("newPass", "Password must be at least 6 characters.");
        }
        else if (confirmedNewPass == null || !confirmedNewPass.equals(newPass)) {
            addFieldError("confirmedNewPass", "Password does NOT match.");
        }
    }

    //
    // stupid method
    //
    private boolean allFieldNull() {
        return oldPass == null && newPass == null && confirmedNewPass == null;
    }

    //
    // getters & setters
    //
    public String getOldPass() {
        return oldPass;
    }

    public void setOldPass(String oldPass) {
        this.oldPass = oldPass;
    }

    public String getNewPass() {
        return newPass;
    }

    public void setNewPass(String newPass) {
        this.newPass = newPass;
    }

    public String getConfirmedNewPass() {
        return confirmedNewPass;
    }

    public void setConfirmedNewPass(String confirmedNewPass) {
        this.confirmedNewPass = confirmedNewPass;
    }

}
