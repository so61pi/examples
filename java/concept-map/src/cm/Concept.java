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
public class Concept {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "CONCEPT_ID")
    private int conceptId;

    @OneToOne
    @JoinColumn(name = "DOCUMENT_ID", referencedColumnName = "DOCUMENT_ID")
    private Document document;

    @Column(name = "ID_CLIENT")
    private int idClient;

    @Column(name = "NAME")
    private String name;

    @Column(name = "X_CLIENT")
    private double xClient;

    @Column(name = "Y_CLIENT")
    private double yClient;

    // ==========================================================================
    public Concept() {
    }

    @Override
    public void finalize() throws Throwable {

    }

    // ==========================================================================
    public void fromJSON(JSONObject json) {
        idClient = json.getInt("id");
        name = json.getString("name");
        xClient = json.getDouble("x");
        yClient = json.getDouble("y");
    }

    public JSONObject toJSON() {
        JSONObject jsonConcept = new JSONObject();
        jsonConcept.put("id", idClient);
        jsonConcept.put("name", name);
        jsonConcept.put("x", xClient);
        jsonConcept.put("y", yClient);
        return jsonConcept;
    }

    // ==========================================================================
    public int getConceptId() {
        return conceptId;
    }

    public void setConceptId(int conceptId) {
        this.conceptId = conceptId;
    }

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

    public double getxClient() {
        return xClient;
    }

    public void setxClient(double xClient) {
        this.xClient = xClient;
    }

    public double getyClient() {
        return yClient;
    }

    public void setyClient(double yClient) {
        this.yClient = yClient;
    }

}