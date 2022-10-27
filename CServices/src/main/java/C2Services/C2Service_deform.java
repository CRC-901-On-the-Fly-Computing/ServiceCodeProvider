package C2Services;

import C2Data.C2Image;
import C2Data.C2NativeInterface;
import C2Data.C2Resource;
import C2Plugins.Plugin;
import C2Data.C2Params;


import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;
import java.util.*;

public class C2Service_deform extends Plugin {

    private static File serviceFile;
    private static List<File> linkedFiles;

    static {
        serviceFile = new File(C2NativeInterface.getPluginDir() + "libservice_deform.so");
        linkedFiles = new ArrayList<File>();
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_deform_cpu.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_deform_scpu.so"));
        //linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_deform_gpu.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_deform_fpga.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_deform_overlay.so"));
    }

    public C2Service_deform() {
        super(serviceFile, linkedFiles);

        Map<String, Double> paramsDefaultMap = new HashMap<String, Double>(){{
            put("radius", 60.0);
            put("angle", 30.0);
            put("position_x", 100.0);
            put("position_y", 100.0);
            put("mode", 1.0);
        }};;
        setOptions(new C2Params(paramsDefaultMap));

        printMethod("__construct");
    }

    protected List<Double> getParamList() {
        List<Double> paramList      = new ArrayList<Double>();
        Map<String,Double> paramMap = getOptions().getParams();

        paramList.add(paramMap.get("radius"));
        paramList.add(paramMap.get("angle"));
        paramList.add(paramMap.get("position_x"));
        paramList.add(paramMap.get("position_y"));
        paramList.add(paramMap.get("mode"));

        return paramList;
    }

    public C2Image processImage(C2Resource resource, C2Image sourceImage) {
        printMethod("processImage", resource.getResourceString());

        switch (resource.getResourceString()) {
            case "s":
            case "c":
            case "g":
            case "f":
                break;
            case "j":
            default:
                throw new Error("Resource '" + resource.getResourceString() + "' not supported by service.");
        }

        List<C2Image> input_images  = new ArrayList<C2Image>();
        input_images.add(sourceImage);

        List<C2Image> output_images = null;

        output_images = (ArrayList<C2Image>) process(resource.getResourceChar(), getParamList(), input_images);

        return output_images.get(0);
    }
}

