package C2Data;

public class C2Resource {
    private String mResource;

    public C2Resource(String resourceName) {
        switch (resourceName) {
            case "c":
            case "C":
            case "cpu":
            case "CPU":
                mResource = "c";
                break;
            case "s":
            case "S":
            case "scpu":
            case "SCPU":
                mResource = "s";
                break;
            case "g":
            case "G":
            case "gpu":
            case "GPU":
                mResource = "g";
                break;
            case "f":
            case "F":
            case "fpga":
            case "FPGA":
                mResource = "f";
                break;
            case "j":
            case "J":
            case "java":
            case "JAVA":
                mResource = "j";
                break;
            case "o":
            case "O":
            case "overlay":
            case "OVERLAY":
                mResource = "o";
                break;
            default:
                throw new AssertionError("Resource '" + resourceName + "' unknown!");
        }
    }

    public char getResourceChar() {
        return mResource.charAt(0);
    }

    public String getResourceString() {
        return mResource;
    }
}
