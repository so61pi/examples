package adaptors;

import org.json.JSONObject;
import org.json.JSONTokener;

public class DrawingData {
    private MapHeader header;
    private MapContent content;

    //
    //
    //
    public void fromJsonString(String jsonString) {
        JSONObject jsonData = new JSONObject(new JSONTokener(jsonString));
        header.fromJson(jsonData.getJSONObject("header"));
        content.fromJson(jsonData.getJSONObject("content"));
    }

    public String toJsonString() {
        JSONObject jsonData = new JSONObject();
        jsonData.put("header", header.toJson());
        jsonData.put("content", content.toJson());
        return jsonData.toString();
    }

    //
    //
    //
    public MapHeader getHeader() {
        return header;
    }

    public void setHeader(MapHeader header) {
        this.header = header;
    }

    public MapContent getContent() {
        return content;
    }

    public void setContent(MapContent content) {
        this.content = content;
    }
}
