package action;

import form.ForgotPasswordForm;
import guard.Login;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import core.User;

public class ForgotPasswordAction extends Action {
    @Override
    public ActionForward execute(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response)
                    throws Exception {
        if (Login.stop(request, response)) {
            return Login.out(mapping);
        }

        ForgotPasswordForm formbean = (ForgotPasswordForm) form;
        HttpSession session = request.getSession();
        synchronized (session) {
            if (formbean == null || formbean.empty()) {
                return mapping.findForward("input");
            }

            User user = User.getUserByEmail(formbean.getEmail());
            User.forgotPass(user);

            request.setAttribute("reason",
                    "A new password has been sent to your email!");
            return mapping.findForward("success");
        }
    }
}
