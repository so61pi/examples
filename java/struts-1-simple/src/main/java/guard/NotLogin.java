package guard;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

public class NotLogin {
    public static boolean stop(HttpServletRequest request,
            HttpServletResponse response) {

        HttpSession session = request.getSession();
        synchronized (session) {
            Integer userId = (Integer) session.getAttribute("userId");
            return userId == null;
        }
    }

    public static ActionForward out(ActionMapping mapping) {
        return mapping.findForward("login");
    }
}
