/*
 * Timer
 */

package DICOM.Misc;

public class Timer {
  public native long timeStruct();
  public native double computeDelta(long timeStruct);
  public native double releaseTimeStruct(long timeStruct);

  static {
    //System.loadLibrary("javaerl");
  }

  protected long TimeStruct;
  protected double delta;

  public Timer() {
    TimeStruct = 0;
    delta = 0.;
  }

  public void start() {
    if (TimeStruct == 0)
      TimeStruct = timeStruct();
  }
  public void stop() {
    delta = computeDelta(TimeStruct);
    releaseTimeStruct(TimeStruct);
    TimeStruct = 0;
  }

  public double deltaTime() {
    return delta;
  }
}
