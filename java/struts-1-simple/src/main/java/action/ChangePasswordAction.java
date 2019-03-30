package action;

import form.ChangePasswordForm;
import guard.NotLogin;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import core.User;

public class ChangePasswordAction extends Action {
    @Override
    public ActionForward execute(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response)
                    throws Exception {
        if (NotLogin.stop(request, response)) {
            return NotLogin.out(mapping);
        }

        ChangePasswordForm formbean = (ChangePasswordForm) form;
        HttpSession session = request.getSession();
        synchronized (session) {
            if (formbean == null || formbean.empty()) {
                return mapping.findForward("input");
            }

            int userId = (Integer) session.getAttribute("userId");
            User.changePass(User.getUserById(userId), formbean.getNewPassword());

            request.setAttribute("reason",
                    "Your password is changed successfully.");
            return mapping.findForward("success");
        }
    }
}
