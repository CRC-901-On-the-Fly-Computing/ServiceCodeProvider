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
import java.util.Map;
import java.util.HashMap;

public class C2Service_contrast extends Plugin {

    private static File serviceFile;
    private static List<File> linkedFiles;

    static {
        serviceFile = new File(C2NativeInterface.getPluginDir() + "libservice_contrast.so");
        linkedFiles = new ArrayList<File>();
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_contrast_cpu.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_contrast_scpu.so"));
        //linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_contrast_gpu.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_contrast_fpga.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_contrast_overlay.so"));
    }

    public C2Service_contrast() {
        super(serviceFile, linkedFiles);

        Map<String, Double> paramsDefaultMap = new HashMap<String, Double>();
        paramsDefaultMap.put("factor", 1.0);
        setOptions(new C2Params(paramsDefaultMap));

        printMethod("__construct");
    }

    protected List<Double> getParamList() {
        List<Double> paramList      = new ArrayList<Double>();
        Map<String,Double> paramMap = getOptions().getParams();

        paramList.add(paramMap.get("factor"));

        return paramList;
    }

    public C2Image processImage(C2Resource resource, C2Image sourceImage) {

        Objects.requireNonNull(resource, "resource argument must not be null.");
        Objects.requireNonNull(sourceImage, "sourceImage argument must not be null.");

        printMethod("processImage", resource.getResourceString());

        List<C2Image> input_images  = Collections.singletonList(sourceImage);
        List<C2Image> output_images = internalProcessImages(resource, input_images);
        assert  output_images.size() == 1;
        return output_images.get(0);
    }

    public List<C2Image> processImages(C2Resource resource, List<C2Image> input_images) {

        Objects.requireNonNull(resource, "resource argument must not be null.");
        Objects.requireNonNull(input_images, "input_images argument must not be null.");

        printMethod("processImages", resource.getResourceString());

        return internalProcessImages(resource, input_images);
    }

    private List<C2Image> internalProcessImages(C2Resource resource, List<C2Image> input_images) {

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

        List<C2Image> output_images = null;

        output_images = new ArrayList<>(input_images.size());
        for (C2Image image : input_images) {
            C2Image filteredImage = ((List<C2Image>) process(resource.getResourceChar(), getParamList(), Collections.singletonList(image))).get(0);
            output_images.add(filteredImage);
        }

        return output_images;
    }
}
