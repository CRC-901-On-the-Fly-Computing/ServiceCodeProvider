package C2Data;

import java.util.*;

public class C2Params {

    private Map< String,Double> mParams;

    public C2Params(Map< String,Double> paramValues) { mParams = paramValues; }

    public Map<String,Double> getParams() { return mParams; }

    public int getSize(){return mParams.size();}
}