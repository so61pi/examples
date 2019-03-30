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
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.OneToOne;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;

/**
 * @author Expired
 * @version 1.0
 * @created 07-Oct-2014 4:00:22 PM
 */
@Entity
public class ConceptMapRepo {

    @ManyToOne
    @JoinColumn(name = "ASSIGNMENT_ID")
    private Assignment assignment;

    @Temporal(TemporalType.DATE)
    @Column(name = "CREATE_DATE")
    private Date createDate = new Date();

    @ManyToOne(cascade = CascadeType.ALL)
    @JoinColumn(name = "CREATOR_ID")
    private User creator;

    @Column(name = "DESCRIPTION")
    private String description;

    @OneToOne
    @JoinColumn(name = "GENERAL_DOCUMENT_ID", referencedColumnName = "DOCUMENT_ID")
    private Document generalDocument;

    @OneToMany(cascade = CascadeType.ALL)
    @JoinColumn(name = "REPO_ID")
    private List<ConceptMap> history = new ArrayList<ConceptMap>();

    @Column(name = "NAME")
    private String name;

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "REPO_ID")
    private int repoId;

    // =========================================================================
    public ConceptMapRepo() {
    }

    @Override
    public void finalize() throws Throwable {
    }

    // =========================================================================
    public static ConceptMapRepo getRepoById(List<ConceptMapRepo> listCmRepo,
            int id) {
        if (listCmRepo != null) {
            for (ConceptMapRepo x : listCmRepo) {
                if (x.getRepoId() == id)
                    return x;
            }
        }
        return null;
    }

    public ConceptMap getLastConceptMap() {
        return null;
    }

    public void addConceptMap(ConceptMap cm) {
        history.add(cm);
    }

    public int grade(ConceptMap cm, StringBuilder result) {
        return 0;
    }

    public void submit() {

    }

    // =========================================================================
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

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public Document getGeneralDocument() {
        return generalDocument;
    }

    public void setGeneralDocument(Document generalDocument) {
        this.generalDocument = generalDocument;
    }

    public List<ConceptMap> getHistory() {
        return history;
    }

    public void setHistory(List<ConceptMap> history) {
        this.history = history;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getRepoId() {
        return repoId;
    }

    public void setRepoId(int repoId) {
        this.repoId = repoId;
    }

}