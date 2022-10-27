package C2Plugins;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

public class ServiceMetaInformation implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = -1343095491120366617L;

	private List<String> resources = new ArrayList<>();

	/**
	 * IDs that the service can be called by.
	 */
	private List<String> ids = new ArrayList<>();

	/**
	 * Information about the service.
	 */
	private List<String> information = new ArrayList<>();

	/**
	 * The available operations on this service mapped to the parameters of an
	 * operation.
	 */
	private Map<String, List<String>> operations = new HashMap<>();

	private int inputNumberAssurance = 0;

	private int outputNumberAssurance = 0;

	public ServiceMetaInformation(List<String> ids, List<String> information, Map<String, List<String>> operations,
			List<String> resources) {
		if (ids != null)
			this.ids = ids;
		if (information != null)
			this.information = information;
		if (operations != null)
			this.operations = operations;
		if (resources != null)
			this.resources = resources;
	}

	public void setInputNumberAssurance(int inputNumberAssurance) {
		this.inputNumberAssurance = inputNumberAssurance;
	}

	public void setOutputNumberAssurance(int outputNumberAssurance) {
		this.outputNumberAssurance = outputNumberAssurance;
	}

	public int getInputNumberAssurance() {
		return inputNumberAssurance;
	}

	public int getOutputNumberAssurance() {
		return outputNumberAssurance;
	}

	public List<String> getResources() {
		return resources;
	}

	public List<String> getIDs() {
		return ids;
	}

	public List<String> getInfo() {
		return information;
	}

	public Map<String, List<String>> getParams() {
		return operations;
	}

	public List<String> getOperations() {
		return new ArrayList<>(operations.keySet());
	}

	public List<String> getParams(String operation) {
		return operations.get(operation);
	}

	public List<String> getQualifiedOperations() {
		List<String> result = new ArrayList<>();
		for (String id : getIDs()) {
			for (String op : getOperations()) {
				result.add(id + "." + op);
			}
		}
		return result;
	}

	public ServiceMetaInformation deepCopy() {
		List<String> idsCopy = new ArrayList<>();
		List<String> informationCopy = new ArrayList<>();
		List<String> resourcesCopy = new ArrayList<>();
		Map<String, List<String>> operationsCopy = new HashMap<>();
		for (String s : ids)
			idsCopy.add(new String(s));
		for (String s : information)
			informationCopy.add(new String(s));
		for (String s : resources)
			resourcesCopy.add(new String(s));
		for (Entry<String, List<String>> e : operations.entrySet()) {
			List<String> params = new ArrayList<>();
			for (String s : e.getValue())
				params.add(new String(s));
			operationsCopy.put(new String(e.getKey()), params);
		}
		ServiceMetaInformation result = new ServiceMetaInformation(idsCopy, informationCopy, operationsCopy,
				resourcesCopy);
		return result;
	}

	@Override
	public String toString() {
		StringBuilder stringBuilder = new StringBuilder();
		stringBuilder.append("\nIDs: ");
		for (int i = 0; i < ids.size() - 1; i++) {
			stringBuilder.append(ids.get(i) + " | ");
		}
		if (ids.size() > 0) {
			stringBuilder.append(ids.get(ids.size() - 1) + "\n");
		} else {
			stringBuilder.append("EMPTY \n");
		}
		stringBuilder.append("\nResources: ");
		for (int i = 0; i < resources.size() - 1; i++) {
			stringBuilder.append(resources.get(i) + " | ");
		}
		if (resources.size() > 0) {
			stringBuilder.append(resources.get(resources.size() - 1) + "\n");
		} else {
			stringBuilder.append("EMPTY \n");
		}
		stringBuilder.append("\nInformation: ");
		for (int i = 0; i < information.size(); i++) {
			stringBuilder.append(information.get(i) + "\n");
		}
		stringBuilder.append("\nOperations: ");
		for (Entry<String, List<String>> entry : operations.entrySet()) {
			stringBuilder.append(entry.getKey() + " >> ");
			if (entry.getValue() != null) {
				for (int i = 0; i < entry.getValue().size() - 1; i++) {
					stringBuilder.append(entry.getValue().get(i) + " | ");
				}
				if (entry.getValue().size() > 0) {
					stringBuilder.append(entry.getValue().get(entry.getValue().size() - 1) + "\n\n");
				} else {
					stringBuilder.append("EMPTY \n");
				}
			} else {
				stringBuilder.append("No parameters");
			}
		}
		if (inputNumberAssurance != 0)
			stringBuilder.append("Input number assurance is set to: " + inputNumberAssurance);
		if (outputNumberAssurance != 0)
			stringBuilder.append("Output number assurance is set to: " + outputNumberAssurance);
		return stringBuilder.toString();
	}
}
