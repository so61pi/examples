package servlets;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.hibernate.Session;
import org.json.JSONObject;

import utils.HibernateUtil;
import utils.Json;
import adaptors.DrawingData;
import cm.ConceptMapRepo;
import cm.User;

/**
 * Servlet implementation class Save
 */
@WebServlet("/save")
public class SaveServlet extends HttpServlet {
    private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public SaveServlet() {
        super();
        // TODO Auto-generated constructor stub
    }

    /**
     * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse
     *      response)
     */
    @Override
    protected void doGet(HttpServletRequest request,
            HttpServletResponse response) throws ServletException, IOException {
        // TODO Auto-generated method stub
    }

    /**
     * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse
     *      response)
     */
    @Override
    protected void doPost(HttpServletRequest request,
            HttpServletResponse response) throws ServletException, IOException {
        String jsonString = Json.readJsonFromInputStream(request
                .getInputStream());

        HttpSession httpSession = request.getSession();
        synchronized (httpSession) {
            Integer userId = (Integer) httpSession.getAttribute("userId");

            DrawingData data = new DrawingData();
            data.fromJsonString(jsonString);

            Session session = HibernateUtil.getSessionFactory().openSession();

            User user = (User) HibernateUtil.get(session, User.class, userId);
            ConceptMapRepo repo = ConceptMapRepo.getRepoById(user.getMyMaps(),
                    data.getHeader().getRepoId());
            if (repo == null) {
                repo = new ConceptMapRepo();
                user.getMyMaps().add(repo);
            }
            data.getHeader().toRepo(repo);
            repo.addConceptMap(data.getContent().toCm());

            HibernateUtil.update(session, user);

            JSONObject x = new JSONObject();
            x.put("repoId", repo.getRepoId());
            Json.writeJsonToOutputStream(response.getOutputStream(),
                    x.toString());

            session.close();
        }
    }
}
