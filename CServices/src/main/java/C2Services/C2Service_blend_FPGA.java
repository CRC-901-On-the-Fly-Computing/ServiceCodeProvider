package C2Services;

import C2Data.C2Image;
import C2Data.C2Params;
import C2Data.C2Resource;

import java.util.Map;
import java.util.Set;

public class C2Service_blend_FPGA extends C2Service_blend{
	private C2Resource mResource;
	public C2Service_blend_FPGA() {
		super();

		mResource = new C2Resource("fpga");
	}

	public C2Image processImage(C2Image sourceImage1, C2Image sourceImage2) {
		return super.processImage(mResource, sourceImage1, sourceImage2);
	}
}
