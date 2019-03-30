package action;

import form.RegisterForm;
import guard.Login;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import core.User;

public class RegisterAction extends Action {
    @Override
    public ActionForward execute(ActionMapping mapping, ActionForm form,
            HttpServletRequest request, HttpServletResponse response)
                    throws Exception {
        if (Login.stop(request, response)) {
            return Login.out(mapping);
        }

        RegisterForm formbean = (RegisterForm) form;
        HttpSession session = request.getSession();
        synchronized (session) {
            if (formbean == null || formbean.empty()) {
                return mapping.findForward("input");
            }

            User.register(formbean.getUserName(), formbean.getPassword(),
                    formbean.getEmail());

            request.setAttribute("reason", "Register successfully!");
            return mapping.findForward("success");
        }
    }
}
