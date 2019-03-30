package cm;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.OneToOne;

import org.json.JSONObject;

/**
 * @author Expired
 * @version 1.0
 * @created 07-Oct-2014 4:00:22 PM
 */
@Entity
public class Relationship {

    @OneToOne
    @JoinColumn(name = "DOCUMENT_ID", referencedColumnName = "DOCUMENT_ID")
    private Document document;

    @Column(name = "ID_CLIENT")
    private int idClient;

    @Column(name = "NAME")
    private String name;

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "RELATIONSHIP_ID")
    private int relationshipId;

    @Column(name = "SOURCE_ID_CLIENT")
    private int sourceIdClient;

    @Column(name = "TARGET_ID_CLIENT")
    private int targetIdClient;

    // ==========================================================================
    public Relationship() {
    }

    @Override
    public void finalize() throws Throwable {
    }

    // ==========================================================================
    public void fromJSON(JSONObject json) {
        idClient = json.getInt("id");
        name = json.getString("name");
        sourceIdClient = json.getInt("source");
        targetIdClient = json.getInt("target");
    }

    public JSONObject toJSON() {
        JSONObject jsonRelationship = new JSONObject();
        jsonRelationship.put("id", idClient);
        jsonRelationship.put("name", name);
        jsonRelationship.put("source", sourceIdClient);
        jsonRelationship.put("target", targetIdClient);
        return jsonRelationship;
    }

    // ==========================================================================
    public Document getDocument() {
        return document;
    }

    public void setDocument(Document document) {
        this.document = document;
    }

    public int getIdClient() {
        return idClient;
    }

    public void setIdClient(int idClient) {
        this.idClient = idClient;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getRelationshipId() {
        return relationshipId;
    }

    public void setRelationshipId(int relationshipId) {
        this.relationshipId = relationshipId;
    }

    public int getSourceIdClient() {
        return sourceIdClient;
    }

    public void setSourceIdClient(int sourceIdClient) {
        this.sourceIdClient = sourceIdClient;
    }

    public int getTargetIdClient() {
        return targetIdClient;
    }

    public void setTargetIdClient(int targetIdClient) {
        this.targetIdClient = targetIdClient;
    }

}