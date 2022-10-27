package C2Data;

import java.util.Map;

public class C2NativeInterface {
    private String mSedeRoot;
    private String mImageBridge;
    private String mPluginBridge;
    private static String mPluginDir;

    private static C2NativeInterface instance;

    static  {
        getInstance ();
    }

    private C2NativeInterface() {
        try {
            Map<String, String> env = System.getenv();

            if (!env.containsKey("SEDE_ROOT")) {
                throw new AssertionError("SEDE_ROOT not set as environment variable!");
            } else {
                mSedeRoot = env.get("SEDE_ROOT");
            }

            System.setProperty("LD_LIBRARY_PATH", mSedeRoot + "/CServices/csrc/service_plugins/build/");
        } catch (SecurityException e) {
            throw new AssertionError("Security policy doesn't allow access to system environment!", e);
        }

        mImageBridge  = new String(mSedeRoot + "/CServices/csrc/imagebridge/build/libimagemagick.so");
        mPluginBridge = new String(mSedeRoot + "/CServices/csrc/pluginbridge/build/libpluginbridge.so");
        mPluginDir    = new String(mSedeRoot + "/CServices/csrc/service_plugins/build/");

        try {
            System.out.println("Load native library: libimagemagick.so");
            System.load(mImageBridge);

            System.out.println("Load native library: libpluginbridge.so");
            System.load(mPluginBridge);
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Cannot load native libraries: " + e.getMessage());
        }
    }

    public static C2NativeInterface getInstance () {
        if (C2NativeInterface.instance == null) {
            C2NativeInterface.instance = new C2NativeInterface();
        }

        return C2NativeInterface.instance;
    }

    public String getSedeRootDir() {
        return mSedeRoot;
    }

    public static String getPluginDir() {
        if (mPluginDir == null) {
            throw new AssertionError("Native Interface not yet intialized.");
        }

        return mPluginDir;
    }
}
