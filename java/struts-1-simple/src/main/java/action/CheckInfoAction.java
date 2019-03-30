package action;

import java.util.Map;

import javax.servlet.http.*;
import org.apache.struts.action.*;

import core.User;
import guard.NotLogin;

public class CheckInfoAction extends Action {
    @Override
    public ActionForward execute(ActionMapping mapping,
                                 ActionForm form,
                                 HttpServletRequest request,
                                 HttpServletResponse response)
            throws Exception {
        if (NotLogin.stop(request, response)) {
            return NotLogin.out(mapping);
        }
        
        HttpSession session = request.getSession();
        synchronized (session) {
            int userId = (Integer) session.getAttribute("userId");
            User user = User.getUserById(userId);
            request.setAttribute("user", user);
            return mapping.findForward("success");
        }
    }
}
