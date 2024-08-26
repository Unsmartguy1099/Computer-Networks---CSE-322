package main;

import java.util.List;

public class FileRequstMessage {
    private String fileName;
    private String fileDescription;
    private List<String> senderList;
    private Boolean resolved;
    private String uploader;

    public FileRequstMessage(String fileName,String fileDescription){
        resolved = false;
        this.fileName = fileName;
        this.fileDescription = fileDescription;

    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public void setFileDescription(String fileDescription) {
        this.fileDescription = fileDescription;
    }

    public void setResolved(Boolean resolved) {
        this.resolved = resolved;
    }

    public String getFileName() {
        return fileName;
    }

    public Boolean getResolved() {
        return resolved;
    }

    public String getFileDescription() {
        return fileDescription;
    }

    public void setSenderList(String from) {
        this.senderList.add(from);
    }

    public List<String> getSenderList() {
        return senderList;
    }

    public void setUploader(String uploader) {
        this.uploader = uploader;
    }

    public String getUploader() {
        return uploader;
    }
}
