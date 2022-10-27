package C2Services;

import C2Data.C2Image;
import C2Data.C2NativeInterface;
import C2Data.C2Params;
import C2Data.C2Resource;
import C2Plugins.Plugin;

import java.io.File;
import java.util.*;
import java.util.stream.Collectors;


public class C2Service_grey extends Plugin {

    private static File serviceFile;
    private static List<File> linkedFiles;

    static {
        serviceFile = new File(C2NativeInterface.getPluginDir() + "libservice_grey.so");
        linkedFiles = new ArrayList<File>();
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_grey_cpu.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_grey_scpu.so"));
        //linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_grey_gpu.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_grey_fpga.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_grey_overlay.so"));
    }

    public C2Service_grey() {
        super(serviceFile, linkedFiles);

        printMethod("__construct");
    }

    protected List<Double> getParamList() {
        List<Double> paramList      = new ArrayList<Double>();

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

        switch (resource.getResourceChar()) {
            case 's':
            case 'c':
            case 'g':
            case 'f':
            case 'j':
            case 'o':
                break;
            default:
                throw new IllegalArgumentException("Resource '" + resource.getResourceString() + "' not supported by service.");
        }

        List<C2Image> output_images = null;

        if (resource.getResourceChar() != 'j') {
            output_images = new ArrayList<>(input_images.size());
            for (C2Image image : input_images) {
                C2Image filteredImage = ((List<C2Image>) process(resource.getResourceChar(), getParamList(), Collections.singletonList(image))).get(0);
                output_images.add(filteredImage);
            }
        } else {
            output_images = rgb2grey(input_images);
        }
        return output_images;
    }

    private List<C2Image> rgb2grey(List<C2Image> inputImages) {
        return inputImages.stream().map(this::rgb2grey).collect(Collectors.toList());
    }

    public C2Image rgb2grey(C2Image inputImage) {
        int lrows           = inputImage.getRows();
        int lcolumns        = inputImage.getColumns();

        short[] pixelsIn    = inputImage.getPixels();
        short[] pixelsOut   = new short[inputImage.getRows() * inputImage.getColumns() * 4];

        int pos = 0;
        for (int i = 0; i < lrows; ++i) {
            for (int j = 0; j < lcolumns; ++j) {
                pos = (i * lcolumns + j) * 4;

                int blue    = pixelsIn[pos + 0] & 0xffff;
                int green   = pixelsIn[pos + 1] & 0xffff;
                int red     = pixelsIn[pos + 2] & 0xffff;

                short value   = (short) ((blue + green + red)/3);

                // blue
                pixelsOut[pos] = value;
                // green
                pixelsOut[pos + 1] = value;
                // red
                pixelsOut[pos + 2] = value;
                // alpha
                pixelsOut[pos + 3] = 0;
            }
        }

        C2Image outputImage = new C2Image(pixelsOut, inputImage.getRows(), inputImage.getColumns());
        return outputImage;
    }
}
