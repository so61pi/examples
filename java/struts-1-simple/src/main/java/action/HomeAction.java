package action;

import guard.NotLogin;
import java.util.Map;

import javax.servlet.http.*;
import org.apache.struts.action.*;

public class HomeAction extends Action {
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
            return mapping.findForward("success");
        }
    }
}
