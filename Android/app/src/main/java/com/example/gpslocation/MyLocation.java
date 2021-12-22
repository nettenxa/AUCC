package com.example.gpslocation;

public class MyLocation {
    private double latitude;
    private double logitude;

    private MyLocation(double latitude, double logitude){
        this.latitude = latitude;
        this.logitude = logitude;
    }
    private MyLocation(){}

    public double getLatitude() {
        return latitude;
    }

    public void setLatitude(double latitude) {
        this.latitude = latitude;
    }

    public double getLogitude() {
        return logitude;
    }

    public void setLogitude(double logitude) {
        this.logitude = logitude;
    }
}
