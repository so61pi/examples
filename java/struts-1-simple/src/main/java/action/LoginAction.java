package action;

import form.LoginForm;
import guard.Login;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import core.User;

public class LoginAction extends Action {
    @Override
    public ActionForward execute(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response)
            throws Exception {
        if (Login.stop(request, response)) {
            return Login.out(mapping);
        }

        LoginForm formbean = (LoginForm) form;
        HttpSession session = request.getSession();
        synchronized (session) {
            if (formbean == null || formbean.empty()) {
                return mapping.findForward("input");
            }

            User user = User.getUserByName(formbean.getUserName());
            session.setAttribute("userId", user.getUserId());

            return mapping.findForward("home");
        }
    }
}
