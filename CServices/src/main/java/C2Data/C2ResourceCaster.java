package C2Data;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import java.io.*;
import java.util.List;

public class C2ResourceCaster {
    public C2Resource cfs_C2Resource(InputStream is) throws Exception {
        JSONParser parser = new JSONParser();
        Reader reader = new InputStreamReader(is);
        JSONObject jsonobject = (JSONObject) parser.parse(reader);

        String resourceType = (String)jsonobject.get("ResourceType");

        C2Resource resource = new C2Resource(resourceType);

        return resource;
    }

    public void cts_C2Resource(OutputStream os, C2Resource resource) throws IOException {
        JSONObject entry = new JSONObject();
        entry.put("ResourceType", resource.getResourceString());

        OutputStreamWriter writer = new OutputStreamWriter(os);
        entry.writeJSONString(writer);
        writer.flush();
    }
}
