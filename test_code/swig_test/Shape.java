/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class Shape {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected Shape(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(Shape obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        swigshapeJNI.delete_Shape(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setX(double value) {
    swigshapeJNI.Shape_x_set(swigCPtr, this, value);
  }

  public double getX() {
    return swigshapeJNI.Shape_x_get(swigCPtr, this);
  }

  public void setY(double value) {
    swigshapeJNI.Shape_y_set(swigCPtr, this, value);
  }

  public double getY() {
    return swigshapeJNI.Shape_y_get(swigCPtr, this);
  }

  public void move(double dx, double dy) {
    swigshapeJNI.Shape_move(swigCPtr, this, dx, dy);
  }

  public double area() {
    return swigshapeJNI.Shape_area(swigCPtr, this);
  }

  public double perimeter() {
    return swigshapeJNI.Shape_perimeter(swigCPtr, this);
  }

  public void setNshapes(int value) {
    swigshapeJNI.Shape_nshapes_set(swigCPtr, this, value);
  }

  public int getNshapes() {
    return swigshapeJNI.Shape_nshapes_get(swigCPtr, this);
  }

}