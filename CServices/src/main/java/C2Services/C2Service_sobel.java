package C2Services;

import C2Data.C2Image;
import C2Data.C2NativeInterface;
import C2Data.C2Params;
import C2Data.C2Resource;
import C2Plugins.Plugin;

import java.io.File;
import java.util.*;
import java.util.stream.Collectors;

public class C2Service_sobel extends Plugin {

    private static File serviceFile;
    private static List<File> linkedFiles;

    static {
        serviceFile = new File(C2NativeInterface.getPluginDir() + "libservice_sobel.so");
        linkedFiles = new ArrayList<File>();
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_sobel_cpu.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_sobel_scpu.so"));
        //linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_sobel_gpu.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_sobel_fpga.so"));
        linkedFiles.add(new File(C2NativeInterface.getPluginDir() + "libservice_sobel_overlay.so"));
    }

    public C2Service_sobel() {
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

        List<C2Image> sourceImages = Collections.singletonList(sourceImage);
        List<C2Image> outputImages = internalProcessImages(resource, sourceImages);
        assert outputImages.size() == 1;

        return outputImages.get(0);
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
            output_images = sobel(input_images);
        }
        return output_images;
    }


    public List<C2Image> sobel(List<C2Image> inputImage) {
        return inputImage.stream()
                .map(this::sobel)
                .collect(Collectors.toList()); // Collectors.toList returns an ArrayList instance
    }

    public C2Image sobel(C2Image inputImage) {
//        C2Image inputImage  = inputImages.get(0);

        int rows            = inputImage.getRows();
        int columns         = inputImage.getColumns();

        short[] pixelsIn    = inputImage.getPixels();
        short[] pixelsInter = new short[inputImage.getRows() * inputImage.getColumns() * 4];
        short[] pixelsOut   = new short[inputImage.getRows() * inputImage.getColumns() * 4];

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                int pos     = (i * columns + j) * 4;

                int blue = pixelsIn[pos + 0] & 0xffff;
                int green = pixelsIn[pos + 1] & 0xffff;
                int red = pixelsIn[pos + 2] & 0xffff;

                int value = blue + green + red;

                pixelsInter[pos + 0] = (short) (value / 3);
                pixelsInter[pos + 1] = pixelsInter[pos + 0];
                pixelsInter[pos + 2] = pixelsInter[pos + 0];
                pixelsInter[pos + 3] = 0;
            }
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                int pos = i * columns + j;

                int temp_red_x, temp_red_y;

                if (i==0 || j==0 || i==rows-1 || j==columns-1) {
                    temp_red_x = 0;
                    temp_red_y = 0;
                } else {
                    temp_red_x =
                            (-1 * (pixelsInter[(pos - columns - 1) * 4] & 0xffff))
                          + (-2 * (pixelsInter[(pos - columns    ) * 4] & 0xffff))
                          + (-1 * (pixelsInter[(pos - columns + 1) * 4] & 0xffff))
                          + (     (pixelsInter[(pos + columns - 1) * 4] & 0xffff))
                          + ( 2 * (pixelsInter[(pos + columns    ) * 4] & 0xffff))
                          + (     (pixelsInter[(pos + columns + 1) * 4] & 0xffff));

                    temp_red_y =
                            (-1 * (pixelsInter[(pos - columns - 1) * 4] & 0xffff))
                          + (     (pixelsInter[(pos - columns + 1) * 4] & 0xffff))
                          + (-2 * (pixelsInter[(pos           - 1) * 4] & 0xffff))
                          + ( 2 * (pixelsInter[(pos           + 1) * 4] & 0xffff))
                          + (-1 * (pixelsInter[(pos + columns - 1) * 4] & 0xffff))
                          + (     (pixelsInter[(pos + columns + 1) * 4] & 0xffff));
                }

                if(true) {
                    pixelsOut[(pos * 4)] = (short) ((Math.abs(temp_red_x) > Math.abs(temp_red_y)) ? Math.abs(temp_red_x) : Math.abs(temp_red_y));
                } else {
                    pixelsOut[(pos * 4)] = (short) (Math.sqrt(temp_red_x * temp_red_x + temp_red_y * temp_red_y));
                }

                pixelsOut[(pos * 4) + 1]	= pixelsOut[(pos * 4)];
                pixelsOut[(pos * 4) + 2]	= pixelsOut[(pos * 4)];
                pixelsOut[(pos * 4) + 3]	= 0;
            }
        }

        C2Image outputImage = new C2Image(pixelsOut, inputImage.getRows(), inputImage.getColumns());
        return outputImage;
    }
}
