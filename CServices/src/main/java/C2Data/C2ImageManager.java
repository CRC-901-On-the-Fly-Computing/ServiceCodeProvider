package C2Data;

import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.util.ArrayList;
import java.util.List;

public class C2ImageManager {
    /* VARIABLES */
    private List<String> mFilenamesSource;
    private List<String> mFilenamesTarget;
    private List<String> mFilenamesExp;

    private List<C2Image> mSourceImages;
    private List<C2Image> mTargetImages;
    private List<C2Image> mExpImages;

    private List<BufferedImage> mSourceBufferedImages;
    private List<BufferedImage> mTargetBufferedImages;

    private C2ImageMagickCaller mIMC;

    /* CONSTRUCTORS */
    public C2ImageManager(List<String> filenamesSource, List<String> filenamesTarget) {
        mFilenamesSource 	= filenamesSource;
        mFilenamesTarget 	= filenamesTarget;

        mSourceImages		= new ArrayList<C2Image>();
        mTargetImages		= new ArrayList<C2Image>();
        mExpImages			= new ArrayList<C2Image>();

        mIMC				= new C2ImageMagickCaller();
    }

    public C2ImageManager(){
        mFilenamesSource 	= new ArrayList<String>();
        mFilenamesTarget 	= new ArrayList<String>();
        mFilenamesExp		= new ArrayList<String>();

        mSourceImages		= new ArrayList<C2Image>();
        mTargetImages		= new ArrayList<C2Image>();
        mExpImages			= new ArrayList<C2Image>();

        mSourceBufferedImages = new ArrayList<BufferedImage>();
        mTargetBufferedImages = new ArrayList<BufferedImage>();

        mIMC				= new C2ImageMagickCaller();

    }

    /* METHODS */
    public void environmentUp() {
        mIMC.imEnvUp();
    }

    public void environmentDown() {
        mIMC.imEnvDown();
    }

    public List<C2Image> getSourceImages() {
        return mSourceImages;
    }

    public BufferedImage getSourceImage(int no){
        return mSourceBufferedImages.get(no);
    }

    public BufferedImage getTargetImage(int no){
        return mTargetBufferedImages.get(no);
    }

    public List<BufferedImage> getTargetImageList(){
        return mTargetBufferedImages;
    }

    public void readImages() {
        for (int i=0; i<mFilenamesSource.size(); ++i) {
            mSourceImages.add(readImage(mFilenamesSource.get(i)));
        }
    }

    public void readExpImages(){
        for (int i=0; i<mFilenamesExp.size(); i++){
            mExpImages.add(readImage(mFilenamesExp.get(i)));
        }
    }

    private C2Image readImage(String filename) {
        long cptrImage	= mIMC.openImage(filename);
        short[] pixPack	= mIMC.readImage(cptrImage);
        int rows		= mIMC.getImageRows(cptrImage);
        int columns		= mIMC.getImageColumns(cptrImage);
        mIMC.closeImage(cptrImage);

        C2Image image = new C2Image(pixPack, rows, columns);

        return image;
    }

    public void setTargetImages(List<C2Image> images) {
        mTargetImages = images;
    }

    public List<C2Image> getTargetImages() {
        return mTargetImages;
    }

    public void writeImages() throws Exception {
        if (mFilenamesTarget.size() != mTargetImages.size()) {
            throw new Exception("!!! WARNING: there are more or less images than filenames these images have to be stored to.");
        }

        for (int i=0; i<mFilenamesTarget.size(); ++i) {
            writeImage(mTargetImages.get(i), mFilenamesTarget.get(i));
        }
    }

    public void writeTargetImage(String imagePath, int no){
        System.out.println("Write Images");
        writeImage(mTargetImages.get(no), imagePath);
    }

    private void writeImage(C2Image image, String filename) {
        long cptrImageTarget = mIMC.openImage(filename);
        mIMC.createImage(cptrImageTarget, image.getRows(), image.getColumns());
        mIMC.writeImage(cptrImageTarget, image.getPixels());
        mIMC.closeImage(cptrImageTarget);
    }

    public void addSourceImage(String imagePath){
        C2Image image = readImage(imagePath);
        mSourceImages.add(image);
        mFilenamesSource.add(imagePath);
        mSourceBufferedImages.add(convertToBufferedImage(image));
    }

    public void addWebcamImage(BufferedImage buffImage){
        C2Image image = convertFromBufferedImage(buffImage);
        mSourceImages.add(image);
        mFilenamesSource.add("Webcam");
        mSourceBufferedImages.add(buffImage);
    }

    public void addTargetImageFilename(String imagePath){
        mFilenamesTarget.add(imagePath);
    }

    public List<String> getTargetImageFilenames(){
        return mFilenamesTarget;
    }

    public List<String> getSourceImageFilenames(){
        return mFilenamesSource;
    }

    public BufferedImage addBuffImgFromTargetList(C2Image img){
        BufferedImage buffImg = convertToBufferedImage(img);
        mTargetBufferedImages.add(buffImg);
        return buffImg;
    }

    public void resetSrcImg(){
        mFilenamesSource.clear();
        mSourceBufferedImages.clear();
        mSourceImages.clear();
    }

    public void resetTarImg(){
        mFilenamesTarget.clear();
        mTargetBufferedImages.clear();
        mTargetImages.clear();
    }

    public static BufferedImage convertToBufferedImage(C2Image image) {
        int pos;
        int rows = image.getRows();
        int columns = image.getColumns();
        short[] pixPack = image.getPixels();
        BufferedImage buffImage = new BufferedImage(columns,  rows, BufferedImage.TYPE_4BYTE_ABGR);
        byte[] pixels = ((DataBufferByte) buffImage.getRaster().getDataBuffer()).getData();
        for (int i = 0; i < rows ; i++){
            for (int j = 0; j < columns; j++){
                pos = (i*columns+j)*4;
                pixels[pos] = (byte) (255);
                pixels[pos+1] = (byte)(((int) pixPack[pos] & 0xffff) >> 8);
                pixels[pos+2] = (byte)(((int) pixPack[pos+1] & 0xffff) >> 8);
                pixels[pos+3] = (byte)(((int) pixPack[pos+2] & 0xffff) >> 8);
            }
        }

        return buffImage;

    }

    public static C2Image convertFromBufferedImage(BufferedImage buffImage) {
        int pos, pos2;

        byte[] pixels = ((DataBufferByte) buffImage.getRaster().getDataBuffer()).getData();
        boolean hasAlphaChannel = (buffImage.getAlphaRaster() != null);
        int rows = buffImage.getHeight();
        int columns = buffImage.getWidth();
        short[] pixPack = new short[columns*rows*4];
        C2Image image = new C2Image(pixPack, rows, columns);

        if(hasAlphaChannel){
            for (int i = 0; i < rows ; i++){
                for (int j = 0; j < columns; j++){
                    pos = (i*columns+j)*4;
                    pixPack[pos] = (short)(((int) pixels[pos+1] & 0xff) << 8);
                    pixPack[pos+1] = (short)(((int) pixels[pos+2] & 0xff) << 8);
                    pixPack[pos+2] = (short)(((int) pixels[pos+3] & 0xff) << 8);
                    pixPack[pos+3] = (short)(((int) pixels[pos] - 255 & 0xff) << 8);
                }
            }
        } else {
            for (int i = 0; i < rows ; i++){
                for (int j = 0; j < columns; j++){
                    pos = (i*columns+j)*4;
                    pos2 = (i*columns+j)*3;
                    pixPack[pos] = (short)(((int) pixels[pos2+0] & 0xff) << 8);
                    pixPack[pos+1] = (short)(((int) pixels[pos2+1] & 0xff) << 8);
                    pixPack[pos+2] = (short)(((int) pixels[pos2+2] & 0xff) << 8);
                    pixPack[pos+3] = (short)(((int) 255 & 0xff) << 8);
                }
            }
        }

        return image;
    }
}

