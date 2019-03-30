package adaptors;

import java.util.Date;

import org.json.JSONObject;

import cm.ConceptMap;

public class MapContent {
    private ConceptMap cm = new ConceptMap();

    //
    //
    //
    public void fromJson(JSONObject json) {
        Date createDate = new Date();
        cm.fromJson(json.getJSONObject("cm"));
        cm.setCreateDate(createDate);
    }

    public ConceptMap toCm() {
        return cm;
    }

    //
    //
    //
    public void fromCm(ConceptMap cm) {
        this.cm = cm;
    }

    public JSONObject toJson() {
        JSONObject json = new JSONObject();
        json.put("createDate", cm.getCreateDate());
        json.put("cm", cm.toJson());
        return json;
    }

    //
    //
    //
    public ConceptMap getCm() {
        return cm;
    }

    public void setCm(ConceptMap cm) {
        this.cm = cm;
    }
}
