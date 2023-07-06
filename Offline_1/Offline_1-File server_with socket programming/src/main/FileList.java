package main;

import java.io.Serializable;
import java.util.List;

public class FileList implements Serializable {
private List<String> files;
private String type;
public FileList(List<String> files, String type){
    this.files=files;
    this.type=type;
}

    public List<String> getFiles() {
        return files;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getType() {
        return type;
    }

    public void setFiles(List<String> files) {
        this.files = files;
    }
}
