package main;

import java.io.Serializable;

public class FileRequest implements Serializable {
    String from;
    String filepath;
    String request;
    String fileName;
    String scope;
    FileRequest(String filepath,String request,String from,String fileName,String scope){
        this.filepath=filepath;
        this.request=request;
        this.from=from;
        this.fileName=fileName;
        this.scope=scope;
    }
    public void setRequest(String request) {
        this.request = request;
    }
    public void setFilepath(String filepath) {
        this.filepath = filepath;
    }
    public String getFilepath() {
        return filepath;
    }
    public String getRequest() {
        return request;
    }
    public String getFrom() {
        return from;
    }
    public void setFrom(String from) {
        this.from = from;
    }
    public void setFileName(String fileName) {
        this.fileName = fileName;
    }
    public String getFileName() {
        return fileName;
    }
    public void setScope(String scope) {
        this.scope = scope;
    }
    public String getScope() {
        return scope;
    }
}
