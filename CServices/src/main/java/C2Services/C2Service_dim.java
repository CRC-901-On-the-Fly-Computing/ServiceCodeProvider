package C2Services;

import C2Data.C2Image;
import C2Data.C2NativeInterface;
import C2Data.C2Resource;
import C2Plugins.Plugin;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

public class C2Service_dim extends Plugin {

    private static File serviceFile;
    private static List<File> linkedFiles;

    static {
        serviceFile = new File(C2NativeInterface.getPluginDir() + "libservice_dim.so");
        linkedFiles = new ArrayList<File>();
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_dim_cpu.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_dim_scpu.so"));
        //linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_dim_gpu.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_dim_fpga.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_dim_overlay.so"));
    }

    public C2Service_dim() {
        super(serviceFile, linkedFiles);

        printMethod("__construct");
    }

    protected List<Double> getParamList() {
        List<Double> paramList      = new ArrayList<Double>();

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

