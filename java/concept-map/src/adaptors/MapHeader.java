package adaptors;

import java.util.Date;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;

import cm.ConceptMapRepo;

public class MapHeader {
    private String name;
    private int repoId;
    private String description;
    private Date createDate;
    private Date deadline;
    private boolean submitted;
    private String creator;

    //
    //
    //
    // data from client...
    public void fromJson(JSONObject json) {
        name = json.getString("name");
        repoId = json.getInt("repoId");
        description = json.getString("description");
        createDate = new Date();
        deadline = new Date(json.getInt("deadline"));
    }

    // ... to server
    public void toRepo(ConceptMapRepo repo) {
        repo.setName(name);
        // repo.setRepoId(repoId);
        repo.setDescription(description);
        repo.setCreateDate(createDate);
        // dealine
        //
        //
    }

    //
    //
    //
    // data from server...
    public void fromRepo(ConceptMapRepo repo) {
        name = repo.getName();
        repoId = repo.getRepoId();
        description = repo.getDescription();
        createDate = repo.getCreateDate();
        deadline = new Date();
        submitted = false;
        creator = repo.getCreator().getUserName();
    }

    // ... to client
    public JSONObject toJson() {
        JSONObject json = new JSONObject();
        json.put("name", name);
        json.put("repoId", repoId);
        json.put("description", description);
        json.put("createDate", createDate.getTime());
        json.put("deadline", deadline.getTime());
        json.put("submitted", submitted);
        json.put("creator", creator);
        return json;
    }

    //
    //
    //
    public static JSONArray fromRepoList(List<ConceptMapRepo> repoList) {
        JSONArray jsonArray = new JSONArray();
        for (ConceptMapRepo r : repoList) {
            MapHeader h = new MapHeader();
            h.fromRepo(r);
            jsonArray.put(h.toJson());
        }
        return jsonArray;
    }

    //
    //
    //
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

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public Date getCreateDate() {
        return createDate;
    }

    public void setCreateDate(Date createDate) {
        this.createDate = createDate;
    }

    public Date getDeadline() {
        return deadline;
    }

    public void setDeadline(Date deadline) {
        this.deadline = deadline;
    }

    public boolean getSubmitted() {
        return submitted;
    }

    public void setSubmitted(boolean submitted) {
        this.submitted = submitted;
    }

    public String getCreator() {
        return creator;
    }

    public void setCreator(String creator) {
        this.creator = creator;
    }
}
