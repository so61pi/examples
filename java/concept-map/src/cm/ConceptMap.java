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
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;

import org.json.JSONArray;
import org.json.JSONObject;

@Entity
public class ConceptMap {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "CM_ID")
    private int conceptMapId;

    @OneToMany(cascade = CascadeType.ALL)
    @JoinColumn(name = "CM_ID")
    private List<Concept> concepts = new ArrayList<Concept>();

    @Temporal(TemporalType.DATE)
    @Column(name = "CREATE_DATE")
    private Date createDate = new Date();;

    @OneToMany(cascade = CascadeType.ALL)
    @JoinColumn(name = "CM_ID")
    private List<Relationship> relationships = new ArrayList<Relationship>();

    @ManyToOne(cascade = CascadeType.ALL)
    @JoinColumn(name = "REPO_ID")
    private ConceptMapRepo repo;

    @ManyToMany(cascade = CascadeType.ALL)
    @JoinTable(name = "USER_CONCEPTMAP", joinColumns = { @JoinColumn(name = "CM_ID", referencedColumnName = "CM_ID") }, inverseJoinColumns = { @JoinColumn(name = "USER_ID", referencedColumnName = "USER_ID") })
    private List<User> to;

    // ==========================================================================
    public ConceptMap() {
    }

    @Override
    public void finalize() throws Throwable {
    }

    // ==========================================================================
    public void fromJson(JSONObject json) {
        concepts = new ArrayList<Concept>();
        relationships = new ArrayList<Relationship>();

        // transform json concept to real concept object, then add to concept
        // list
        JSONArray jsonConceptArray = json.getJSONArray("concepts");
        for (int i = 0; i < jsonConceptArray.length(); i++) {
            Concept c = new Concept();
            c.fromJSON((JSONObject) jsonConceptArray.get(i));
            concepts.add(c);
        }

        // transform json relationship to real relationship object, then add to
        // relationship list
        JSONArray jsonRelationshipArray = json.getJSONArray("relationships");
        for (int i = 0; i < jsonRelationshipArray.length(); i++) {
            Relationship r = new Relationship();
            r.fromJSON((JSONObject) jsonRelationshipArray.get(i));
            relationships.add(r);
        }
    }

    public JSONObject toJson() {
        JSONObject jsonCm = new JSONObject();

        // convert all concepts to JSONObject and put to JSONArray
        JSONArray jsonConceptArray = new JSONArray();
        for (Concept c : concepts) {
            jsonConceptArray.put(c.toJSON());
        }
        // put newly created JSONArray of concepts to json of map
        jsonCm.put("concepts", jsonConceptArray);

        // convert all relationships to JSONObject and put to JSONArray
        JSONArray jsonRelationshipArray = new JSONArray();
        for (Relationship r : relationships) {
            jsonRelationshipArray.put(r.toJSON());
        }
        // put newly created JSONArray of relationships to json of map
        jsonCm.put("relationships", jsonRelationshipArray);

        return jsonCm;
    }

    // ==========================================================================
    public int getConceptMapId() {
        return conceptMapId;
    }

    public void setConceptMapId(int conceptMapId) {
        this.conceptMapId = conceptMapId;
    }

    public List<Concept> getConcepts() {
        return concepts;
    }

    public void setConcepts(List<Concept> concepts) {
        this.concepts = concepts;
    }

    public Date getCreateDate() {
        return createDate;
    }

    public void setCreateDate(Date createDate) {
        this.createDate = createDate;
    }

    public List<Relationship> getRelationships() {
        return relationships;
    }

    public void setRelationships(List<Relationship> relationships) {
        this.relationships = relationships;
    }

    public ConceptMapRepo getRepo() {
        return repo;
    }

    public void setRepo(ConceptMapRepo repo) {
        this.repo = repo;
    }

    public List<User> getTo() {
        return to;
    }

    public void setTo(List<User> to) {
        this.to = to;
    }

}