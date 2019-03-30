package cm;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.JoinTable;
import javax.persistence.ManyToMany;
import javax.persistence.OneToOne;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;

/**
 * @author Expired
 * @version 1.0
 * @created 07-Oct-2014 4:00:22 PM
 */
@Entity
public class Assignment {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "ASSIGNMENT_ID")
    private int assignmentId;

    @Temporal(TemporalType.DATE)
    @Column(name = "CREATE_DATE")
    private Date createDate;

    @OneToOne
    @JoinColumn(name = "CREATOR")
    private User creator;

    @Temporal(TemporalType.DATE)
    @Column(name = "DEADLINE")
    private Date deadline;

    @OneToOne
    @JoinColumn(name = "SOLUTION_REPO_ID", referencedColumnName = "REPO_ID")
    private ConceptMapRepo solution;

    @ManyToMany(cascade = CascadeType.ALL)
    @JoinTable(name = "USER_ASSIGNMENT", joinColumns = { @JoinColumn(name = "ASSIGNMENT_ID", referencedColumnName = "ASSIGNMENT_ID") }, inverseJoinColumns = { @JoinColumn(name = "USER_ID", referencedColumnName = "USER_ID") })
    private List<User> students = new ArrayList<User>();

    @Temporal(TemporalType.DATE)
    @Column(name = "SUBMIT_DATE")
    private Date submitDate;

    // ==========================================================================
    public Assignment() {
    }

    @Override
    public void finalize() throws Throwable {
    }

    // ==========================================================================
    public void addStudent(User student) {
        students.add(student);
    }

    public void removeStudent(User student) {
        for (int i = 0; i < students.size(); i++) {
            if (students.get(i).getUserId() == student.getUserId()) {
                students.remove(i);
            }
        }
    }

    // ==========================================================================
    public int getAssignmentId() {
        return assignmentId;
    }

    public void setAssignmentId(int assignmentId) {
        this.assignmentId = assignmentId;
    }

    public Date getCreateDate() {
        return createDate;
    }

    public void setCreateDate(Date createDate) {
        this.createDate = createDate;
    }

    public User getCreator() {
        return creator;
    }

    public void setCreator(User creator) {
        this.creator = creator;
    }

    public Date getDeadline() {
        return deadline;
    }

    public void setDeadline(Date deadline) {
        this.deadline = deadline;
    }

    public ConceptMapRepo getSolution() {
        return solution;
    }

    public void setSolution(ConceptMapRepo solution) {
        this.solution = solution;
    }

    public List<User> getStudents() {
        return students;
    }

    public void setStudents(List<User> students) {
        this.students = students;
    }

    public Date getSubmitDate() {
        return submitDate;
    }

    public void setSubmitDate(Date submitDate) {
        this.submitDate = submitDate;
    }

}