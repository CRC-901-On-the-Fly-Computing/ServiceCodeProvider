package C2Plugins;

import java.io.File;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import C2Data.C2Image;
import C2Data.C2Params;

import java.util.ArrayList;

/**
 * Core class to deal as the java sided representation of a plug-in which is
 * actually implemented in C. Most of the functionality here is obtained by
 * using JNI calls on costruction time to read out the C file. Once the class is
 * constructed the getters can be used without making additional JNI calls. The
 * produced header file by javah is plugins_Plugin.h but the only class that
 * implements it is plugin_bridge.c. It further makes use of methodes of the
 * header-file plugin.h which is then implemented by the individual plug-ins. So
 * the communication pipeline can be seen as {Plugin.javah -> [plugins_Plugin.h]
 * -> plugin_bridge.c -> [plugin.h] -> plugin_XYZ.c}.
 */
abstract public class Plugin extends Service {

	private long mHandler;
	private long mHandlerGPU;

	/**
	 * In the process of constructing this java representation of a plug-in the JNI
	 * calls are used to receive all kind of information about the plug-in
	 * which is actually implemented in C.
	 * @param serviceFile The main plug-in file.
	 * 
	 * @param linkedFiles The various implementations for different resources.
	 *       
	 */
	public Plugin(File serviceFile, List<File> linkedFiles) {
		super(serviceFile, linkedFiles);
		mHandler = c_loadLibrary(serviceFile.getAbsolutePath());
		System.out.println("Load native library: " + serviceFile.getName());
		// mHandlerGPU = c_loadLibraryGPU(mHandler,
		// mPluginFile.getAbsolutePath().replace(".so", "_gpu.so"));

		List<String> ids = new ArrayList<String>(Arrays.asList(c_getServiceIDs(mHandler)));
		List<String> info = new ArrayList<String>(Arrays.asList(c_getServiceInfo(mHandler)));
		List<String> params = new ArrayList<String>(Arrays.asList(c_getServiceParams(mHandler)));
		List<String> resources = new ArrayList<String>(Arrays.asList(c_getServiceResources(mHandler)));
		HashMap<String, List<String>> operationParams = new HashMap<>();
		operationParams.put("process", params);
		ServiceMetaInformation serviceMetaInfos = new ServiceMetaInformation(ids, info, operationParams, resources);
		setMetaInfos(serviceMetaInfos);
		serviceMetaInfos.setInputNumberAssurance(c_getNumberOfInputImages(mHandler));
		serviceMetaInfos.setOutputNumberAssurance(c_getNumberOfOutputImages(mHandler));
		//printLoad();
	}

	/* METHODS */
	public Object process(char resource, List<Double> params, List<C2Image> images) {
		return c_process(mHandler, mHandlerGPU, resource, params, images);
	}

	/**
	 * Prints a string to signal load.
	 */
	public void printLoad() {
		print(new String("+++"));
	}

	/**
	 * Prints a string to signal availability.
	 */
	public void printAvailable() {
		print(new String("ooo"));
	}

	/**
	 * Prints a string to signal unload.
	 */
	public void printUnload() {
		print(new String("---"));
	}

	private void print(String state) {
		System.out.print(state + " service plugin   : " + serviceFile.getName());
		for (int i = 0; i < linkedFiles.size(); ++i) {
			System.out.print("\n    service file " + (i + 1) + "   : " + linkedFiles.get(i).getName());
		}
		System.out.print("\n    service ids      : ");
		printStringList(getMetaInfos().getIDs());
		System.out.print("\n    service info     : ");
		printStringList(getMetaInfos().getInfo());
		System.out.print("\n    service params   : ");
		printStringList(getMetaInfos().getParams("process"));
		System.out.print("\n    service resources: ");
		printStringList(getMetaInfos().getResources());
		System.out.print("\n\n");
	}

	private void printStringList(List<String> stringList) {
		for (int i = 0; i < stringList.size(); ++i) {
			if (i < stringList.size() - 1) {
				System.out.print(stringList.get(i) + ",");
			} else {
				System.out.print(stringList.get(i));
			}
		}
	}

	abstract protected List<Double> getParamList();

	/**
	 * Returns which resources are supported by this plug-in.
	 * 
	 * @return The resources that are supported by this plug-in.
	 */
	public List<String> getResources() {
		return getMetaInfos().getResources();
	}

	public List<String> getParameters() {
		return getMetaInfos().getParams("process");
	}

	/**
	 * Returns the plug-in files that an instance of this class represents.
	 * 
	 * @return The corresponding plug-in file.
	 */
	public File getServiceFile() {
		return serviceFile;
	}

	/**
	 * Returns the files that are given to be the input of the plug-in which is
	 * being called.
	 * 
	 * @return The files that are given to be the input of the plug-in which is
	 *         being called.
	 */
	public List<File> getLinkedFiles() {
		return linkedFiles;
	}

	/**
	 * Returns how many input images are required by this plug-in.
	 * 
	 * @return The number of input images that are required by this plug-in.
	 */
	public int getNumberOfInputImages() {
		return getMetaInfos().getInputNumberAssurance();
	}

	/**
	 * Returns how many output images are returned by this plug-in.
	 * 
	 * @return The number of output images that are produced by this plug-in.
	 */
	public int getNumberOfOutputImages() {
		return getMetaInfos().getOutputNumberAssurance();
	}

	/**
	 * Unloads the library pointed to by the handler and the handler to the GPU, if
	 * it's not null.
	 */
	public void unloadLibraries() {
		c_unloadLibraries(mHandler, mHandlerGPU);
	}

	/**
	 * Dynamically load a library.
	 * 
	 * @param libraryName
	 *            Name of library to load.
	 * @return handle Handle to the library.
	 */
	protected native synchronized long c_loadLibrary(String libraryName);

	/**
	 * Returns a handle to the required library if the plug-in supports the GPU.
	 * 
	 * @param handler
	 *            Handler to the plug-in of interest.
	 * @param libraryName
	 *            Name of library to load.
	 * @return handle Handle to the library.
	 */
	protected native synchronized long c_loadLibraryGPU(long handler, String libraryName);

	/**
	 * Unloads the library pointed to by the handler and the handler to the GPU, if
	 * it's not null.
	 * 
	 * @param handler
	 *            Handler to the plug-in of interest.
	 * @param handlerGPU
	 *            Handler to communicate to the GPU.
	 */
	protected native synchronized void c_unloadLibraries(long handler, long handlerGPU);

	/**
	 * Returns the different acronyms that the plug-in can be called with. (see
	 * plugin.h)
	 * 
	 * @param handler
	 *            Handler to the plug-in of interest.
	 * @return Short names to call a plug-in with.
	 */
	protected native synchronized String[] c_getServiceIDs(long handler);

	/**
	 * Returns a short description of the requested plug-in. (see plugin.h)
	 * 
	 * @param handler
	 *            Handler to the plug-in of interest.
	 * @return Brief description of the requested plug-in.
	 */
	protected native synchronized String[] c_getServiceInfo(long handler);

	/**
	 * Returns which parameters this plug-in offers. (see plugin.h)
	 * 
	 * @param handler
	 *            Handler to the plug-in of interest.
	 * @return Returns an array of the parameters that the given plug-in offers.
	 */
	protected native synchronized String[] c_getServiceParams(long handler);

	/**
	 * Returns which resources are supported by the given plug-in. (see plugin.h)
	 * 
	 * @param handler
	 *            Handler to the plug-in of interest.
	 * @return The resources that are supported by the plug-in.
	 */
	protected native synchronized String[] c_getServiceResources(long handler);

	/**
	 * Initiates the plug-in and lets it run on the declared resource.
	 * 
	 * @param handler
	 *            Handler to plug-in to be run.
	 * @param handlerGPU
	 *            Handler to the GPU (this is typically the same for all plug-in).
	 * @param resource
	 *            Resource to run the plug-in on.
	 * @param params
	 *            Parameters to start the plug-in with.
	 * @param images
	 *            Input images to be processed by the plug-in.
	 * @return Processed resulting images of the plug-in.
	 */
	protected native List<C2Image> c_process(long handler, long handlerGPU, char resource, List<Double> params,
			List<C2Image> images);

	/**
	 * Returns how many input images are required by the plug-in. (see plugin.h)
	 * 
	 * @param handler
	 *            Handler to the plug-in of interest.
	 * @return The number of input images that are required by the plug-in.
	 */
	protected native synchronized int c_getNumberOfInputImages(long handler);

	/**
	 * Returns how many output images are returned by the plug-in. (see plugin.h)
	 * 
	 * @param handler
	 *            Handler to the plug-in of interest.
	 * @return The number of output images that are produced by the plug-in.
	 */
	protected native synchronized int c_getNumberOfOutputImages(long handler);
}
