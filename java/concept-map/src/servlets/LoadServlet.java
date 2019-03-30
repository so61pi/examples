package servlets;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.hibernate.Session;
import org.json.JSONObject;
import org.json.JSONTokener;

import utils.HibernateUtil;
import utils.Json;

/**
 * Servlet implementation class LoadServlet
 */
@WebServlet("/load")
public class LoadServlet extends HttpServlet {
    private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public LoadServlet() {
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
        //
    }

    /**
     * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse
     *      response)
     */
    @Override
    protected void doPost(HttpServletRequest request,
            HttpServletResponse response) throws ServletException, IOException {
        // TODO Auto-generated method stub
        String inputJsonString = Json.readJsonFromInputStream(request
                .getInputStream());

        JSONObject jsonCm = new JSONObject(new JSONTokener(inputJsonString));
        int cmId = jsonCm.getInt("cmId");

        Session session = HibernateUtil.getSessionFactory().openSession();
        // ConceptMap cm = (ConceptMap) HibernateUtil.get(session,
        // ConceptMap.class, cmId);
        // Json.writeJsonToOutputStream(response.getOutputStream(),
        // cm.toJsonString());
        session.close();
    }

}
