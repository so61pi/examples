package cm;

import java.util.ArrayList;
import java.util.List;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;
import javax.persistence.OneToMany;

import org.hibernate.Session;

import utils.HibernateUtil;

@Entity
public class User {

    @ManyToMany(mappedBy = "students", cascade = CascadeType.ALL)
    private List<Assignment> myAssignments = new ArrayList<Assignment>();;

    @OneToMany(cascade = CascadeType.ALL)
    @JoinColumn(name = "CREATOR_ID")
    private List<ConceptMapRepo> myMaps = new ArrayList<ConceptMapRepo>();;

    @ManyToMany(mappedBy = "to", cascade = CascadeType.ALL)
    private List<ConceptMap> myShareToMeMaps = new ArrayList<ConceptMap>();

    @Column(name = "USERNAME")
    private String userName;

    @Column(name = "PASSWORD")
    private String password;

    @Column(name = "ROLE")
    private int role;

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "USER_ID")
    private int userId;

    // ==========================================================================
    public User() {
    }

    @Override
    public void finalize() throws Throwable {
    }

    // ==========================================================================
    public void addStudents(List<User> students, ConceptMapRepo repo) {

    }

    public static boolean authenticate(String userName, String password,
            int role) {
        boolean result = false;

        Session session = HibernateUtil.getSessionFactory().openSession();

        User user = (User) HibernateUtil.findFirst(User.class, "userName",
                userName);
        if (user != null) {
            boolean checkPassword = user.getPassword().equals(password);
            boolean checkRole = user.getRole() == role;
            if (checkPassword & checkRole)
                result = true;
            else
                result = false;
        }
        else {
            result = false;
        }

        session.close();

        return result;
    }

    public void shareConceptMap(ConceptMap cm, User user) {
        cm.getTo().add(user);
    }

    public void submitConceptMap(ConceptMapRepo repo) {
        repo.submit();
    }

    // ==========================================================================
    public List<Assignment> getMyAssignments() {
        return myAssignments;
    }

    public void setMyAssignments(List<Assignment> myAssignments) {
        this.myAssignments = myAssignments;
    }

    public List<ConceptMapRepo> getMyMaps() {
        return myMaps;
    }

    public void setMyMaps(List<ConceptMapRepo> myMaps) {
        this.myMaps = myMaps;
    }

    public List<ConceptMap> getMyShareToMeMaps() {
        return myShareToMeMaps;
    }

    public void setMyMapsShareToMeMaps(List<ConceptMap> myShareToMeMaps) {
        this.myShareToMeMaps = myShareToMeMaps;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public int getRole() {
        return role;
    }

    public void setRole(int role) {
        this.role = role;
    }

    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

}