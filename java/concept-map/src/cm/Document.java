package cm;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;

/**
 * @author Expired
 * @version 1.0
 * @created 07-Oct-2014 4:00:22 PM
 */
@Entity
public class Document {

    @Column(name = "DESCRIPTION")
    private String description;

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "DOCUMENT_ID")
    private int documentId;

    @Column(name = "FILE_LINK")
    private String fileLink;

    @Column(name = "NAME")
    private String name;

    // ==========================================================================
    public Document() {

    }

    @Override
    public void finalize() throws Throwable {

    }

    // ==========================================================================
    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public int getDocumentId() {
        return documentId;
    }

    public void setDocumentId(int documentId) {
        this.documentId = documentId;
    }

    public String getFileLink() {
        return fileLink;
    }

    public void setFileLink(String fileLink) {
        this.fileLink = fileLink;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

}