package C2Services;

import C2Data.C2Image;
import C2Data.C2Params;
import C2Data.C2Resource;

import java.util.List;
import java.util.Map;
import java.util.Set;

public class C2Service_grey_JAVA extends C2Service_grey{
	private C2Resource mResource;
	public C2Service_grey_JAVA() {
		super();

		mResource = new C2Resource("java");
	}

	public C2Image processImage(C2Image sourceImage) {
		return super.processImage(mResource, sourceImage);
	}

	public List<C2Image> processImages(List<C2Image> sourceImages) {
		return super.processImages(mResource, sourceImages);
	}
}
