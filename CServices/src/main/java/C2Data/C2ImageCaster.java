package C2Data;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.Reader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;

import C2Data.C2Image;
import org.json.simple.parser.ParseException;

public class C2ImageCaster {

	public JSONObject encodeC2Image(C2Image image) throws IOException {
		JSONObject entry = new JSONObject();
		entry.put("rows", image.getRows());
		entry.put("columns", image.getColumns());

		/*
		 * This caster needs to translate the image pixels from the C2Image class to match the expected image encoding (entry->pixels).
		 *
		 * The expected image encoding array:
		 *         - The array is one dimensional and the size is equal to the amount of pixel multiplied by 4.
		 *         - Lists each pixel starting from top-left, row-wise.
		 *         - Every pixel is defined by 4 numeric values in [0 : 255] each value represents one channel value. The order is Alpha, Red, Green, Blue.
		 *         - Alpha encodes transparency and the value 255 (MAX VALUE) means zero transparency i.e. the colors are visible.
		 *         - Red green blue are color channels and the value 255 (MAX VALUE) means the color is maxed out, e.g. (r,g,b) = (255,255,255) is the color white.
		 *
		 * The C2Image pixel array encodes images in the following way:
		 *         - The array is one dimensional and the size is equal to the amount of pixel multiplied by 4.
		 *         - Lists each pixel starting from top-left, row-wise.
		 *         - Every pixel is defined by 4 numeric values in [-32768 : 32767] each value represents one channel value. The order is Blue, Green, Red, Alpha.
		 *         - The channels are encoded in unsigned shorts. E.g. 0 is the lowest value, -1 is the highest value and -32768 and 32767 are approximately half the max value.
		 *           Note:
		 *           		C2Image treats java short as unsigned 16-bit integers.
		 *           		As a result -1=(1111111111111111)b is the highest value. The second highest value is -2=(1111111111111110)b and -3=(1111111111111101)b is the third highest value and so on.
		 *           		Incrementing Short.MAX_VALUE which is 32767=(0111111111111111)b, overflows as a result to -32768 = (1000000000000000)b. This is in-fact the intended behaviour.
		 *         - Alpha encodes transparency and the value -1 (MAX VALUE) means full transparency i.e. the colors are NOT visible.
		 *         - Red green blue are color channels and the value -1 (MAX VALUE) means the color is maxed out, e.g. (r,g,b) = (-1,-1,-1) is the color white.
		 *
		 *
		 * The following steps have to be taken to translate one C2 pixel (B G R A) to expected pixel encoding (A R G B):
		 *         - Reorder channels.
		 *         - Right shift each channel value by 8.
		 *              Examples:
		 *                  -    0 will remain 0.
		 *                  -32768 = (1000000000000000)b will be shifted to (0000000010000000)b = 128
		 *                  -    1 = (1111111111111111)b will be shifted to (0000000011111111)b = 255
		 *         - Flip the alpha channel by subtracting 255  with the value of the Alpha channel.
		 *              Examples:
		 *                  0   -> 255 -   0 = 255
		 *                  128 -> 255 - 128 = 127
		 *                  255 -> 255 - 255 =   0
		 *
		 */


		short[] pixelArr = image.getPixels();

		/*
		 * First pass is calculate image type. Assume its gray and hasAlphaChannel.
		 *   - image type is gray iff isColored == false and hasAlphaChannel == false
		 *   - image type is rgb  iff isColored ==  true and hasAlphaChannel == false
		 *   - image type is argb iff                        hasAlphaChannel ==  true
		 */
		boolean isColored = false,
				hasAlphaChannel = false;

		for (int index = 0, size = pixelArr.length;
			 index < size; ) {
			
			isColored = isColored  // if a pixel has been found to be colored the image type degrades to color.
						|| pixelArr[index] != pixelArr[index++]  // check if the first and second color channel are equal
						|| pixelArr[index++] != pixelArr[index++] ; // check if the second and third color channel are equal.
			hasAlphaChannel = pixelArr[index++] != 0; // check if the alpha channel is 0, i.e. the pixel is not transparent.
			if(hasAlphaChannel) // the first transpare
				break;
		}

		List<Integer> expectedImageEncoding;
		if( (!isColored) && (!hasAlphaChannel)) {
			entry.put("imagetype", "Grayscale");
			expectedImageEncoding = new ArrayList<>((int)(image.getPixels().length / 4)); 
			for (int index = 0, size = pixelArr.length;
				index < size; ) { 
				int greyScaleValue = (pixelArr[index] >> 8) & 255;
				index += 4; // skip to the next pixel
				expectedImageEncoding.add(greyScaleValue); // gray value
			}
		} else if (isColored && !hasAlphaChannel) {
			entry.put("imagetype", "RGB");
			expectedImageEncoding = new ArrayList<>((int)(((float) image.getPixels().length) * (0.75f)));
			for (int index = 0, size = pixelArr.length;
				 index < size; ) {

				int blue = (pixelArr[index++] >> 8) & 255;
				int green = (pixelArr[index++] >> 8) & 255;
				int red = (pixelArr[index] >> 8) & 255;
				index+= 2; // skip the alpha channel

				expectedImageEncoding.add(red); // red
				expectedImageEncoding.add(green); // green
				expectedImageEncoding.add(blue); // blue
			}
		}
		else {
			entry.put("imagetype", "ARGB");
			expectedImageEncoding = new ArrayList<>((int)(((float) image.getPixels().length) * (0.75f)));
			for (int index = 0, size = pixelArr.length;
				 index < size; ) {

				int blue = (pixelArr[index++] >> 8) & 255;
				int green = (pixelArr[index++] >> 8) & 255;
				int red = (pixelArr[index++] >> 8) & 255;
				int alpha = 255 - ((pixelArr[index++] >> 8) & 255);

				expectedImageEncoding.add(alpha); // alpha
				expectedImageEncoding.add(red); // red
				expectedImageEncoding.add(green); // green
				expectedImageEncoding.add(blue); // blue
			}
		}


		entry.put("pixels", expectedImageEncoding);
		return entry;
	}

	public C2Image decodeC2Image(Map jsonobject) {
		/*
		 * Read the block comment in 'encodeC2Image' to understand the encoding process.
		 * This method does the same in reverse.
		 */
		int rows = ((Number) jsonobject.get("rows")).intValue();
		int columns = ((Number) jsonobject.get("columns")).intValue();
		String imagetype = (String) jsonobject.get("imagetype");
		List<Number> encodedPixels = (List) jsonobject.get("pixels");

		short[] decodedPixels = new short[rows * columns * 4];

		if (imagetype.equals("Grayscale")) {
			int pixelOffset = 0;
			for (Number pixel : encodedPixels) {
				short colorChannel = (short) ((pixel.shortValue() << 8) & 65535);
				short alphaChannel = 0;
				decodedPixels[pixelOffset + 0] = colorChannel; // blue
				decodedPixels[pixelOffset + 1] = colorChannel; // green
				decodedPixels[pixelOffset + 2] = colorChannel; // red
				decodedPixels[pixelOffset + 3] = alphaChannel; // alpha
				pixelOffset += 4;
			}
		} else if (imagetype.equals("ARGB")) {
			int pixelOffset = 0;
			Iterator<Number> colorChannelIterator = encodedPixels.iterator();
			while (colorChannelIterator.hasNext()) {
				short alpha = (short) (((255 - colorChannelIterator.next().intValue()) << 8) & 65535);
				short red = (short) ((colorChannelIterator.next().intValue() << 8) & 65535);
				short green = (short) ((colorChannelIterator.next().intValue() << 8) & 65535);
				short blue = (short) ((colorChannelIterator.next().intValue() << 8) & 65535);

				decodedPixels[pixelOffset + 0] = blue; // blue
				decodedPixels[pixelOffset + 1] = green; // green
				decodedPixels[pixelOffset + 2] = red; // red
				decodedPixels[pixelOffset + 3] = alpha; // alpha
				pixelOffset += 4;
			}
		} else if (imagetype.equals("RGB")) {
			int pixelOffset = 0;
			Iterator<Number> colorChannelIterator = encodedPixels.iterator();
			while (colorChannelIterator.hasNext()) {
				short alpha = 0;
				short red = (short) ((colorChannelIterator.next().intValue() << 8) & 65535);
				short green = (short) ((colorChannelIterator.next().intValue() << 8) & 65535);
				short blue = (short) ((colorChannelIterator.next().intValue() << 8) & 65535);

				decodedPixels[pixelOffset + 0] = blue; // blue
				decodedPixels[pixelOffset + 1] = green; // green
				decodedPixels[pixelOffset + 2] = red; // red
				decodedPixels[pixelOffset + 3] = alpha; // alpha
				pixelOffset += 4;
			}
		} else {
			throw new RuntimeException("Image type not supported: " + imagetype);
		}
		return new C2Image(decodedPixels, rows, columns);
	}


	public void cts_C2Image(OutputStream os, C2Image image) throws IOException {
		OutputStreamWriter writer = new OutputStreamWriter(os);
		encodeC2Image(image).writeJSONString(writer);
		writer.flush();
	}

	public C2Image cfs_C2Image(InputStream is) throws Exception {
		JSONParser parser = new JSONParser();
		Reader reader = new InputStreamReader(is);
		JSONObject jsonobject = (JSONObject) parser.parse(reader);
		return decodeC2Image(jsonobject);
	}

	/**
	 * Casts a list of C2Image objects to the semantic representation 'images' and writes the data into the provided stream.
	 */
	public void cts_C2ImageList(OutputStream os, List<C2Image> images) throws IOException {
		ArrayList<JSONObject> encodedImageList = new ArrayList<JSONObject>(images.size());
		for (C2Image image : images) {
			encodedImageList.add((JSONObject) encodeC2Image(image));
		}
		OutputStreamWriter writer = new OutputStreamWriter(os);
		JSONArray.writeJSONString(encodedImageList, writer);
		writer.flush();
	}

	/**
	 * Casts from the semantic representation 'images' to List of C2Image objects.
	 * The data in form of semantic type 'images' is taken from the provided inputstream.
	 */
	public List<C2Image> cfs_C2ImageList(InputStream is) throws ParseException, IOException {
		JSONParser parser = new JSONParser();
		Reader reader = new InputStreamReader(is);
		List<JSONObject> encodedImageList = (JSONArray) parser.parse(reader);
		List<C2Image> bitmapList = new ArrayList<>(encodedImageList.size());
		for (JSONObject encodedImage : encodedImageList) {
			bitmapList.add(decodeC2Image(encodedImage));
		}
		return bitmapList;
	}
}