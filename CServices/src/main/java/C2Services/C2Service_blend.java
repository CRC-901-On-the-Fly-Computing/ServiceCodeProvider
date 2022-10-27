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

public class C2Service_blend extends Plugin {

    private static File serviceFile;
    private static List<File> linkedFiles;

    static {
        serviceFile = new File(C2NativeInterface.getPluginDir() + "libservice_blend.so");
        linkedFiles = new ArrayList<File>();
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_blend_cpu.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_blend_scpu.so"));
        //linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_blend_gpu.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_blend_fpga.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_blend_overlay.so"));
    }

    public C2Service_blend() {
        super(serviceFile, linkedFiles);
        Map<String, Double> paramsDefaultMap = new HashMap<String, Double>();
        paramsDefaultMap.put("dirX", 5.0);
        paramsDefaultMap.put("dirY", 30.0);
        paramsDefaultMap.put("position_x", 0.0);
        paramsDefaultMap.put("position_y", 0.0);
        paramsDefaultMap.put("mode", 1.0);

        setOptions(new C2Params(paramsDefaultMap));

        printMethod("__construct");
    }

    protected List<Double> getParamList() {
        List<Double> paramList      = new ArrayList<Double>();
        Map<String,Double> paramMap = getOptions().getParams();

        paramList.add(paramMap.get("dirX"));
        paramList.add(paramMap.get("dirY"));
        paramList.add(paramMap.get("position_x"));
        paramList.add(paramMap.get("position_y"));
        paramList.add(paramMap.get("mode"));

        return paramList;
    }

    public C2Image processImage(C2Resource resource, C2Image sourceImage1, C2Image sourceImage2) {
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
        input_images.add(sourceImage1);
        input_images.add(sourceImage2);

        List<C2Image> output_images = null;

        output_images = (ArrayList<C2Image>) process(resource.getResourceChar(), getParamList(), input_images);

        return output_images.get(0);
    }
}
