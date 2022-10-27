package C2Data;

import de.upb.sede.util.FileUtil;

public class C2ImageMagickCaller {
    /* CONSTRUCTORS */
    protected C2ImageMagickCaller() {
        //System.loadLibrary("imagemagick");
        // System.load("/home/deffel/coding/SEDE/SEDE.services/c2.image/src/test/resources/shared_libs/libimagemagick.so");
        //System.load(FileUtil.getPathOfResource("shared_libs/libimagemagick.so"));
    }

    /* NATIVE FUNCTIONS */
    private native synchronized void im_env_up();
    private native synchronized void im_env_down();

    private native synchronized long open_image(String filename);
    private native synchronized void close_image(long cptrImage);

    private native synchronized void create_image(long cptrImage, int rows, int columns);
    private native synchronized short[] read_image(long cptrImage);
    private native synchronized void write_image(long cptrImage, short[] pixPack);

    private native synchronized int get_image_rows(long cptrImage);
    private native synchronized int get_image_columns(long cptrImage);

    private native synchronized void set_new_path(long cptrImage, String filename);

    /* METHODS */
    protected void imEnvUp() {
        im_env_up();
    }

    protected void imEnvDown() {
        im_env_down();
    }

    protected long openImage(String filename) {
        return open_image(filename);
    }

    protected void closeImage(long cptrImage) {
        close_image(cptrImage);
    }

    protected void createImage(long cptrImage, int rows, int columns) {
        create_image(cptrImage, rows, columns);
    }

    protected short[] readImage(long cptrImage) {
        return read_image(cptrImage);
    }

    protected void writeImage(long cptrImage, short[] pixPack) {
        write_image(cptrImage, pixPack);
    }

    protected int getImageRows(long cptrImage) {
        return get_image_rows(cptrImage);
    }

    protected int getImageColumns(long cptrImage) {
        return get_image_columns(cptrImage);
    }

    protected void setNewPath(long cptrImage, String filename) {
        set_new_path(cptrImage, filename);
    }

}
