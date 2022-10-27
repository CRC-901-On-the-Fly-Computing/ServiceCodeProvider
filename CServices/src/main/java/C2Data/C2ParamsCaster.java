package C2Data;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import java.io.*;
import java.util.*;
import java.util.Map;

public class C2ParamsCaster {
    public C2Params cfs_C2Params(InputStream is) throws Exception {
        JSONParser parser       = new JSONParser();
        Reader reader           = new InputStreamReader(is);
        JSONObject jsonobject   = (JSONObject) parser.parse(reader);

        Map<String, Double> paramsInternal  = new HashMap<String,Double>();
        Set<String> paramKeys               = jsonobject.keySet();

        for (String key : paramKeys) {
            Double paramValue  = (Double)jsonobject.get(key);
            paramsInternal.put(key, paramValue);
        }

        C2Params params = new C2Params(paramsInternal);

        return params;
    }

    public void cts_C2Params(OutputStream os, C2Params params) throws IOException {
        JSONObject entry = new JSONObject();

        Map<String, Double> paramMap    = params.getParams();
        Set<String> paramKeys           = paramMap.keySet();

        for (String key : paramKeys) {
            double paramValue = paramMap.get(key);
            entry.put(key, paramValue);
        }

        OutputStreamWriter writer = new OutputStreamWriter(os);
        entry.writeJSONString(writer);
        writer.flush();
    }
}
