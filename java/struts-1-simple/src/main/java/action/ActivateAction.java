package action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import utils.Crypto;
import utils.Hex;
import utils.Status;
import core.User;

public class ActivateAction extends Action {
    @Override
    public ActionForward execute(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response)
                    throws Exception {
        String code = request.getParameter("code");
        if (code != null) {
            String userName = Crypto.decrypt(Hex.toByteArray(code));
            if (User.isUserExisting(userName) == Status.USER_EXISTING) {
                User.activateUser(User.getUserByName(userName));
                request.setAttribute("reason", userName
                        + ", your account is fully activated.");
                return mapping.findForward("success");
            }
        }
        request.setAttribute("reason", "Please re-check the URL.");
        return mapping.findForward("error");
    }
}
