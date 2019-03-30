package core;

import java.io.Serializable;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

import org.hibernate.Session;

import utils.Const;
import utils.Crypto;
import utils.Hasher;
import utils.Hex;
import utils.HibernateUtil;
import utils.MailPerson;
import utils.MailSender;
import utils.Status;

@Entity
public class User implements Serializable {
    private static final long serialVersionUID = 4694947515729132803L;

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "USER_ID")
    private int userId;

    @Column(name = "USERNAME")
    private String userName;

    @Column(name = "HASHPASS")
    private String hashPass;

    @Column(name = "EMAIL")
    private String email;

    @Column(name = "ROLE")
    private int role;

    @Column(name = "IS_ACTIVATED")
    private boolean isActivated;

    //
    public static Status isUserExisting(String userName) {
        if (getUserByName(userName) != null)
            return Status.USER_EXISTING;
        else
            return Status.USER_NOT_EXISTING;
    }

    public static Status isEmailExisting(String email) {
        if (getUserByEmail(email) != null)
            return Status.EMAIL_EXISTING;
        else
            return Status.EMAIL_NOT_EXISTING;
    }

    public static User getUserByName(String userName) {
        return (User) HibernateUtil.findFirst(User.class, "userName", userName);
    }

    public static User getUserByEmail(String email) {
        return (User) HibernateUtil.findFirst(User.class, "email", email);
    }

    public static User getUserById(int userId) {
        Session session = HibernateUtil.getSessionFactory().openSession();
        User user = (User) HibernateUtil.get(session, User.class, userId);
        session.close();
        return user;
    }

    public static Status register(String userName, String pass, String email) {
        try {
            if (isUserExisting(userName) == Status.USER_EXISTING)
                return Status.USER_EXISTING;

            if (isEmailExisting(email) == Status.EMAIL_EXISTING)
                return Status.EMAIL_EXISTING;

            User user = new User();
            user.setUserName(userName);
            user.setHashPass(Hasher.md5(pass));
            user.setEmail(email);
            user.setRole(0);
            user.setIsActivated(false);

            // save user to database
            Session session = HibernateUtil.getSessionFactory().openSession();
            HibernateUtil.save(session, user);
            session.close();

            // send activating email
            MailPerson sender = new MailPerson(Const.adminEmail,
                    Const.adminDescription);
            MailPerson receiver = new MailPerson(user.getEmail(),
                    user.getUserName());
            String message = Const.protocol + "://" + Const.address
                    + Const.contextPath + "/Activate.do?code="
                    + Hex.toHexString(Crypto.encrypt(user.getUserName()));
            MailSender.send(sender, receiver, "Activating Mail", message);

            return Status.SUCCESS;
        }
        catch (Exception e) {
            return Status.ERROR;
        }
    }

    public static Status authenticate(String userName, String pass) {
        try {
            if (isUserExisting(userName) == Status.USER_NOT_EXISTING) {
                return Status.USER_NOT_EXISTING;
            }

            User user = getUserByName(userName);
            if (!user.getHashPass().equalsIgnoreCase(Hasher.md5(pass)))
                return Status.PASSWORD_NOT_MATCH;

            return Status.SUCCESS;
        }
        catch (Exception e) {
            return Status.ERROR;
        }
    }

    public static void activateUser(User user) {
        user.setIsActivated(true);

        Session session = HibernateUtil.getSessionFactory().openSession();
        HibernateUtil.update(session, user);
        session.close();
    }

    public static void changePass(User user, String pass) {
        user.setHashPass(Hasher.md5(pass));

        Session session = HibernateUtil.getSessionFactory().openSession();
        HibernateUtil.update(session, user);
        session.close();
    }

    public static void forgotPass(User user) {
        String newPass = Hasher.sha256(user.getHashPass());
        user.setHashPass(Hasher.md5(newPass));

        Session session = HibernateUtil.getSessionFactory().openSession();
        HibernateUtil.update(session, user);
        session.close();

        MailPerson sender = new MailPerson(Const.adminEmail,
                Const.adminDescription);
        MailPerson receiver = new MailPerson(user.getEmail(),
                user.getUserName());
        String message = "New password: " + newPass;
        MailSender.send(sender, receiver, "Forgot Password Mail", message);
    }

    //
    // getters & setters
    //
    public int getUserId() {
        return userId;
    }

    public void setUserId(int userId) {
        this.userId = userId;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getHashPass() {
        return hashPass;
    }

    public void setHashPass(String hashPass) {
        this.hashPass = hashPass;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public int getRole() {
        return role;
    }

    public void setRole(int role) {
        this.role = role;
    }

    public boolean getIsActivated() {
        return isActivated;
    }

    public void setIsActivated(boolean isActivated) {
        this.isActivated = isActivated;
    }
}
